using System;
using System.Threading;

class Simulator
{
    static void Main(string[] args)
    {
        if (args.Length != 5)
        {
            Console.WriteLine("Usage: Simulator <rows> <cols> <nThreads> <nOperations> <mssleep>");
            return;
        }

        // Parse command-line arguments
        int nRows = int.Parse(args[0]);
        int nCols = int.Parse(args[1]);
        int nThreads = int.Parse(args[2]);
        int nOperations = int.Parse(args[3]);
        int mssleep = int.Parse(args[4]);

        // Create the spreadsheet
        SharableSpreadSheet spreadsheet = new SharableSpreadSheet(nRows, nCols);

        // Fill the spreadsheet with random strings
        PopulateSpreadsheet(spreadsheet, nRows, nCols);

        // Log initial state of the spreadsheet
        Console.WriteLine("Simulation begins...");
        Console.WriteLine($"Initial Spreadsheet of size ({nRows},{nCols}):");
        spreadsheet.Print();
        Console.WriteLine("");

        // Create and start threads (users)
        Thread[] threads = new Thread[nThreads];
        for (int i = 0; i < nThreads; i++)
        {
            threads[i] = new Thread(() => SimulateUser(spreadsheet, nOperations, mssleep));
            threads[i].Start();
        }

        // Wait for all threads to complete
        foreach (var thread in threads)
        {
            thread.Join();
        }

        // Log final state of the spreadsheet
        var size = spreadsheet.GetSize();
        Console.WriteLine("");
        Console.WriteLine("Simulation complete.");
        Console.WriteLine($"Final Spreadsheet of size ({size.Item1},{size.Item2}):");
        spreadsheet.Print();
    }

    // Populate the spreadsheet with random strings
    static void PopulateSpreadsheet(SharableSpreadSheet spreadsheet, int nRows, int nCols)
    {
        Random random = new Random();
        for (int i = 0; i < nRows; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                spreadsheet.SetCell(i, j, $"testcell{i + 1}{j + 1}");
            }
        }
    }

    static bool UseCaseSensitive()
    {
        Random random = new Random();
        int use = random.Next(1);
        if (use == 1) {return true;}
        return false;
    }

    // Simulate a user performing random operations
    static void SimulateUser(SharableSpreadSheet spreadsheet, int nOperations, int mssleep)
    {
        Random random = new Random();
        for (int i = 0; i < nOperations; i++)
        {
            bool caseSensitive = UseCaseSensitive();
            int operation = random.Next(13); // Updated range for more operations
            int row = random.Next(spreadsheet.GetSize().Item1);
            int col = random.Next(spreadsheet.GetSize().Item2);
            string randomStr = "RandomStr" + random.Next(100); // Random strings
            int colEnd = random.Next(col, spreadsheet.GetSize().Item2); // Random end column for range search

            switch (operation)
            {
                case 0: // GetCell
                    string cellValue = spreadsheet.GetCell(row, col);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Retrieved \"{cellValue}\" from cell [{row},{col}].");
                    break;

                case 1: // SetCell
                    spreadsheet.SetCell(row, col, randomStr);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Set cell [{row},{col}] to \"{randomStr}\".");
                    break;

                case 2: // SearchString
                    var pos = spreadsheet.SearchString(randomStr);
                    if (pos != null)
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Found \"{randomStr}\" at cell [{pos.Item1},{pos.Item2}].");
                    else
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] \"{randomStr}\" not found.");
                    break;

                case 3: // ExchangeRows
                    int rowToExchange = random.Next(spreadsheet.GetSize().Item1);
                    spreadsheet.ExchangeRows(row, rowToExchange);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Exchanged rows [{row}] and [{rowToExchange}].");
                    break;

                case 4: // ExchangeCols
                    int colToExchange = random.Next(spreadsheet.GetSize().Item2);
                    spreadsheet.ExchangeCols(col, colToExchange);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Exchanged columns [{col}] and [{colToExchange}].");
                    break;

                case 5: // SearchInRow
                    int colFoundInRow = spreadsheet.SearchInRow(row, randomStr);
                    if (colFoundInRow != -1)
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Found \"{randomStr}\" in row [{row}] at column [{colFoundInRow}].");
                    else
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] \"{randomStr}\" not found in row [{row}].");
                    break;

                case 6: // SearchInCol
                    int rowFoundInCol = spreadsheet.SearchInCol(col, randomStr);
                    if (rowFoundInCol != -1)
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Found \"{randomStr}\" in column [{col}] at row [{rowFoundInCol}].");
                    else
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] \"{randomStr}\" not found in column [{col}].");
                    break;

                case 7: // SearchInRange
                    var rangePos = spreadsheet.SearchInRange(col, colEnd, row, row, randomStr);
                    if (rangePos != null)
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Found \"{randomStr}\" in range at cell [{rangePos.Item1},{rangePos.Item2}].");
                    else
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] \"{randomStr}\" not found in range [{row},{col}] to [{row},{colEnd}].");
                    break;

                case 8: // AddRow
                    spreadsheet.AddRow(row);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Added a new row after row [{row}].");
                    break;

                case 9: // AddCol
                    spreadsheet.AddCol(col);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Added a new column after column [{col}].");
                    break;

                case 10: // FindAll 
                    var foundPositions = spreadsheet.FindAll(randomStr, caseSensitive);
                    foreach (var position in foundPositions)
                    {
                        Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Found \"{randomStr}\" at cell [{position.Item1},{position.Item2}].");
                    }
                    break;

                case 11: // SetAll
                    spreadsheet.SetAll("testcell", randomStr, caseSensitive);
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Set all occurrences of \"testcell\" to \"{randomStr}\".");
                    break;

                case 12: // GetSize
                    var size = spreadsheet.GetSize();
                    Console.WriteLine($"User [{Thread.CurrentThread.ManagedThreadId}]: [{DateTime.Now:HH:mm:ss}] Spreadsheet size is [{size.Item1} rows, {size.Item2} columns].");
                    break;

                default:
                    break;
            }

            Thread.Sleep(mssleep); // Sleep between operations


        }


    }
}
