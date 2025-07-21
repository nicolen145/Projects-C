using System;
using System.IO;
using System.Diagnostics;

namespace CPU_Process
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // Get the current process and its PID
            Process currentProcess = Process.GetCurrentProcess();
            int pid = currentProcess.Id;

            // Retrieve the number of iterations from command-line arguments
            string numberString = args[0];
            int n = Convert.ToInt32(numberString);

            // Create a stopwatch to measure execution time
            Stopwatch stopWatch = new Stopwatch();
            Console.WriteLine("Running intensive calculations...");
            stopWatch.Start();

            // Perform intensive calculations
            MakeArrays(n);

            stopWatch.Stop();
            // Display the elapsed time in milliseconds
            Console.WriteLine($"Time for {n} iterations: {stopWatch.ElapsedMilliseconds} ms");

            // Define the path for the output file, named by the process PID
            string path = $"C:\\Users\\Nicole\\source\\repos\\CPU-Process\\CPU-Process\\bin\\Debug\\process_outputs\\{pid}.txt";

            // Save the elapsed time to the file
            using (StreamWriter sw = File.CreateText(path))
            {
                sw.WriteLine(stopWatch.ElapsedMilliseconds); 
            }
        }

        // Function that performs intensive calculations by creating and filling arrays
        static void MakeArrays(int n)
        {
            Random rnd = new Random();
            for (int i = 1; i <= n; i++)
            {
                double[] resultArray = new double[i];
                for (int j = 0; j < i; j++)
                {
                    double r = rnd.NextDouble();
                    resultArray[j] = Math.Sqrt(Math.Sin(r) * Math.Cos(r) * Math.Pow(r, j));
                }

                //Console.WriteLine($"Array {i}: [{string.Join(", ", resultArray)}]");
            }
        }
    }
}
