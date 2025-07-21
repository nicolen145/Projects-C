using System;
using System.Drawing;  
using System.Threading;
using System.Collections.Generic;
using System.IO;
using SixLabors.ImageSharp.Formats.Png;
using System.Diagnostics;  

class ImageSearch
{
    static void Main(string[] args)
    {
        // Argument count check
        if (args.Length != 4)
        {
            Console.WriteLine("Usage: ImageSearch <image1> <image2> <nThreads> <algorithm>");
            return;
        }

        string image1Path = args[0];
        string image2Path = args[1];
        string nThreadsStr = args[2];
        string algorithm = args[3].ToLower();

        // Check if image1 file exists
        if (!File.Exists(image1Path))
        {
            Console.WriteLine($"Error: The file '{image1Path}' does not exist.");
            return;
        }

        // Check if image2 file exists
        if (!File.Exists(image2Path))
        {
            Console.WriteLine($"Error: The file '{image2Path}' does not exist.");
            return;
        }

        // Check if nThreads is a valid integer and greater than or equal to 1
        if (!int.TryParse(nThreadsStr, out int nThreads) || nThreads < 1)
        {
            Console.WriteLine("Error: <nThreads> must be a valid integer (1 or greater).");
            return;
        }

        // Validate algorithm type
        if (algorithm != "exact" && algorithm != "euclidean")
        {
            Console.WriteLine("Error: <algorithm> must be either 'exact' or 'euclidean'.");
            return;
        }

        // Load the images
        Bitmap largeImage = new Bitmap(image1Path);
        Bitmap smallImage = new Bitmap(image2Path);

        // Convert images to Color[][] arrays
        Color[][] largeImageArray = ConvertImageToColorArray(largeImage);
        Color[][] smallImageArray = ConvertImageToColorArray(smallImage);

        // Create a list to store the coordinates of matches
        List<(int x, int y)> matches = new List<(int, int)>();

        // Divide image1 into regions for nThreads
        int width = largeImage.Width;
        int height = largeImage.Height;
        int chunkHeight = height / nThreads; // Chunk for each Theard

        // Multithreading
        Thread[] threads = new Thread[nThreads];

        // Thread work method
        void Search(int startY, int endY)
        {
            for (int y = startY; y < endY; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (algorithm == "exact")
                    {
                        if (ExactMatch(largeImageArray, smallImageArray, x, y)) // If exact match found
                        {
                            matches.Add((x, y)); 
                        }
                    }
                    else if (algorithm == "euclidean")
                    {
                        if (EuclideanMatch(largeImageArray, smallImageArray, x, y)) // If euclidean match found
                        {
                            matches.Add((x, y));
                        }
                    }
                }
            }
        }

        // Start threads
        for (int i = 0; i < nThreads; i++)
        {
            int startY = i * chunkHeight;
            int endY = (i == nThreads - 1) ? height : startY + chunkHeight;
            threads[i] = new Thread(() => Search(startY, endY)); 
            threads[i].Start();
        }

        // Wait for all threads to finish
        foreach (Thread thread in threads)
        {
            thread.Join();
        }

        // Output the results
        foreach (var match in matches)
        {
            Console.WriteLine($"{match.x},{match.y}");
        }
    }

    // Exact match algorithm
    static bool ExactMatch(Color[][] largeImage, Color[][] smallImage, int startX, int startY)
    {
        // Ensure the chunk fits within the bounds
        if (startX + smallImage[0].Length > largeImage[0].Length || startY + smallImage.Length > largeImage.Length)
            return false;

        for (int y = 0; y < smallImage.Length; y++)
        {
            for (int x = 0; x < smallImage[0].Length; x++)
            {
                if (largeImage[startY + y][startX + x] != smallImage[y][x])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Euclidean match algorithm
    static bool EuclideanMatch(Color[][] largeImage, Color[][] smallImage, int startX, int startY)
    {
        // Ensure the chunk fits within the bounds
        if (startX + smallImage[0].Length > largeImage[0].Length || startY + smallImage.Length > largeImage.Length)
            return false;

        double threshold = 0.0;  // exact match

        for (int y = 0; y < smallImage.Length; y++)
        {
            for (int x = 0; x < smallImage[0].Length; x++)
            {
                Color c1 = largeImage[startY + y][startX + x];
                Color c2 = smallImage[y][x];

                double distance = Math.Sqrt(Math.Pow(c1.R - c2.R, 2) + Math.Pow(c1.G - c2.G, 2) + Math.Pow(c1.B - c2.B, 2));
                if (distance > threshold)
                    return false;
            }
        }

        return true;
    }

    // Converts a Bitmap image to a 2D array
    public static Color[][] ConvertImageToColorArray(Bitmap image)
    {
        int width = image.Width;
        int height = image.Height;

        Color[][] colorArray = new Color[height][];

        for (int y = 0; y < height; y++)
        {
            colorArray[y] = new Color[width];
            for (int x = 0; x < width; x++)
            {
                colorArray[y][x] = image.GetPixel(x, y);
            }
        }

        return colorArray;
    }
}
