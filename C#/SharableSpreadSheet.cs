using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

    class SharableSpreadSheet
    {
        private string[,] cells;
        private CustomReaderWriterLock[] rowLocks;
        private CustomReaderWriterLock[] colLocks;
        private int rows;
        private int cols;
        private Mutex globalMutex = new Mutex();
        private Semaphore searchSemaphore;

        /// <summary>
        /// Initializes a spreadsheet of size nRows x nCols.
        /// nUsers sets the limit on concurrent searches, with -1 meaning no limit.
        /// </summary>
        public SharableSpreadSheet(int nRows, int nCols, int nUsers = -1)
        {
            if (nRows <= 0 || nCols <= 0)
                throw new ArgumentException("Number of rows and columns must be positive integers.");

            rows = nRows;
            cols = nCols;
            cells = new string[rows, cols];
            rowLocks = new CustomReaderWriterLock[rows];
            colLocks = new CustomReaderWriterLock[cols];

            for (int i = 0; i < rows; i++)
                rowLocks[i] = new CustomReaderWriterLock();

            for (int j = 0; j < cols; j++)
                colLocks[j] = new CustomReaderWriterLock();

            searchSemaphore = nUsers > 0 ? new Semaphore(nUsers, nUsers) : null;
        }

        /// <summary>
        /// Returns the string at the specified cell [row, col].
        /// </summary>
        public string GetCell(int row, int col)
        {
            ValidateIndex(row, col);

            rowLocks[row].EnterReadLock();
            colLocks[col].EnterReadLock();

            try
            {
                return cells[row, col];
            }
            finally
            {
                colLocks[col].ExitReadLock();
                rowLocks[row].ExitReadLock();
            }
        }

        /// <summary>
        /// Sets the string at the specified cell [row, col].
        /// </summary>
        public void SetCell(int row, int col, string str)
        {
            ValidateIndex(row, col);
            if (str == null) throw new ArgumentNullException(nameof(str), "String cannot be null.");

            rowLocks[row].EnterWriteLock();
            colLocks[col].EnterWriteLock();

            try
            {
                cells[row, col] = str;
            }
            finally
            {
                colLocks[col].ExitWriteLock();
                rowLocks[row].ExitWriteLock();
            }
        }

        /// <summary>
        /// Searches for the first cell containing the specified string and returns its position.
        /// </summary>
        public Tuple<int, int> SearchString(string str)
        {
            if (str == null) throw new ArgumentNullException(nameof(str), "Search string cannot be null.");
            searchSemaphore?.WaitOne();

            try
            {
                for (int i = 0; i < rows; i++)
                {
                    rowLocks[i].EnterReadLock();
                    try
                    {
                        for (int j = 0; j < cols; j++)
                        {
                            colLocks[j].EnterReadLock();
                            try
                            {
                                if (cells[i, j] == str)
                                    return Tuple.Create(i, j);
                            }
                            finally
                            {
                                colLocks[j].ExitReadLock();
                            }
                        }
                    }
                    finally
                    {
                        rowLocks[i].ExitReadLock();
                    }
                }
            }
            finally
            {
                searchSemaphore?.Release();
            }

            return null;
        }

        /// <summary>
        /// Searches for the specified string in the specified row and returns the column index.
        /// Returns -1 if the string is not found.
        /// </summary>
        public int SearchInRow(int row, string str)
        {
            ValidateIndex(row);
            if (str == null) throw new ArgumentNullException(nameof(str), "Search string cannot be null.");
            searchSemaphore?.WaitOne();
            try
            {
                rowLocks[row].EnterReadLock();
                try
                {
                    for (int j = 0; j < cols; j++)
                    {
                        if (cells[row, j] == str)
                            return j; // Return the index if found
                    }
                }
                finally
                {
                    rowLocks[row].ExitReadLock();
                }
                return -1;// Return -1 if not found
            }
            finally { searchSemaphore?.Release(); }
        }

        /// <summary>
        /// Searches for the specified string in the specified column and returns the row index.
        /// Returns -1 if the string is not found.
        /// </summary>
        public int SearchInCol(int col, string str)
        {
            ValidateIndex(col, true);
            if (str == null) throw new ArgumentNullException(nameof(str), "Search string cannot be null.");
            searchSemaphore?.WaitOne();
            try
            {
                colLocks[col].EnterReadLock();
                try
                {
                    for (int i = 0; i < rows; i++)
                    {
                        if (cells[i, col] == str)
                            return i; // Return the index if found
                    }
                }
                finally
                {
                    colLocks[col].ExitReadLock();
                }
                return -1; // Return -1 if not found
            }
            finally { searchSemaphore?.Release(); }
        }

        /// <summary>
        /// Searches for the specified string within the specified range and returns its position.
        /// Returns null if the string is not found.
        /// </summary>
        public Tuple<int, int> SearchInRange(int col1, int col2, int row1, int row2, string str)
        {
            if (str == null) throw new ArgumentNullException(nameof(str), "Search string cannot be null.");
            ValidateIndex(row1, col1);
            ValidateIndex(row2, col2);
            if (col1 > col2 || row1 > row2)
                throw new ArgumentException("Invalid range specified.");
            searchSemaphore?.WaitOne();
            try
            {

                for (int i = row1; i <= row2; i++)
                {
                    rowLocks[i].EnterReadLock();
                    try
                    {
                        for (int j = col1; j <= col2; j++)
                        {
                            colLocks[j].EnterReadLock();
                            try
                            {
                                if (cells[i, j] == str)
                                    return Tuple.Create(i, j); // Return the position if found
                            }
                            finally
                            {
                                colLocks[j].ExitReadLock();
                            }
                        }
                    }
                    finally
                    {
                        rowLocks[i].ExitReadLock();
                    }
                }
                return null; // Return null if not found
            }
            finally { searchSemaphore?.Release(); }
        }

        /// <summary>
        /// Exchanges the contents of the two specified rows.
        /// </summary>
        public void ExchangeRows(int row1, int row2)
        {
            ValidateIndex(row1);
            ValidateIndex(row2);
            if (row1 == row2) return; // No need to exchange identical rows

            rowLocks[row1].EnterWriteLock();
            rowLocks[row2].EnterWriteLock();

            try
            {
                for (int j = 0; j < cols; j++)
                {
                    string temp = cells[row1, j];
                    cells[row1, j] = cells[row2, j];
                    cells[row2, j] = temp;
                }
            }
            finally
            {
                rowLocks[row2].ExitWriteLock();
                rowLocks[row1].ExitWriteLock();
            }
        }

        /// <summary>
        /// Exchanges the contents of the two specified columns.
        /// </summary>
        public void ExchangeCols(int col1, int col2)
        {
            ValidateIndex(col1, true);
            ValidateIndex(col2, true);
            if (col1 == col2) return; // No need to exchange identical columns

            colLocks[col1].EnterWriteLock();
            colLocks[col2].EnterWriteLock();

            try
            {
                for (int i = 0; i < rows; i++)
                {
                    string temp = cells[i, col1];
                    cells[i, col1] = cells[i, col2];
                    cells[i, col2] = temp;
                }
            }
            finally
            {
                colLocks[col2].ExitWriteLock();
                colLocks[col1].ExitWriteLock();
            }
        }

        /// <summary>
        /// Adds a row after the specified row.
        /// </summary>
        public void AddRow(int row1)
        {
            if (row1 < 0 || row1 >= rows) throw new IndexOutOfRangeException("Row index is out of range.");

            globalMutex.WaitOne();

            try
            {
                string[,] newCells = new string[rows + 1, cols];

                for (int i = 0; i <= row1; i++)
                    for (int j = 0; j < cols; j++)
                        newCells[i, j] = cells[i, j];

                for (int i = row1 + 1; i < rows + 1; i++)
                    for (int j = 0; j < cols; j++)
                        newCells[i, j] = cells[i - 1, j];

                cells = newCells;
                Array.Resize(ref rowLocks, rows + 1);
                rowLocks[rows] = new CustomReaderWriterLock();
                rows++;
            }
            finally
            {
                globalMutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Adds a column after the specified column.
        /// </summary>
        public void AddCol(int col1)
        {
            if (col1 < 0 || col1 >= cols) throw new IndexOutOfRangeException("Column index is out of range.");

            globalMutex.WaitOne();

            try
            {
                string[,] newCells = new string[rows, cols + 1];

                for (int i = 0; i < rows; i++)
                    for (int j = 0; j <= col1; j++)
                        newCells[i, j] = cells[i, j];

                for (int i = 0; i < rows; i++)
                    for (int j = col1 + 1; j < cols + 1; j++)
                        newCells[i, j] = cells[i, j - 1];

                cells = newCells;
                Array.Resize(ref colLocks, cols + 1);
                colLocks[cols] = new CustomReaderWriterLock();
                cols++;
            }
            finally
            {
                globalMutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Searches for all cells containing the specified string and returns their positions.
        /// </summary>
        public Tuple<int, int>[] FindAll(string str, bool caseSensitive)
        {
            if (str == null) throw new ArgumentNullException(nameof(str), "Search string cannot be null.");
            searchSemaphore?.WaitOne();

            List<Tuple<int, int>> results = new List<Tuple<int, int>>();

            try
            {
                for (int i = 0; i < rows; i++)
                {
                    rowLocks[i].EnterReadLock();
                    try
                    {
                        for (int j = 0; j < cols; j++)
                        {
                            colLocks[j].EnterReadLock();
                            try
                            {
                                string cellContent = cells[i, j];
                                if ((caseSensitive && cellContent == str) ||
                                    (!caseSensitive && string.Equals(cellContent, str, StringComparison.OrdinalIgnoreCase)))
                                {
                                    results.Add(Tuple.Create(i, j));
                                }
                            }
                            finally
                            {
                                colLocks[j].ExitReadLock();
                            }
                        }
                    }
                    finally
                    {
                        rowLocks[i].ExitReadLock();
                    }
                }
            }
            finally
            {
                searchSemaphore?.Release();
            }

            return results.ToArray();
        }

        /// <summary>
        /// Replaces all occurrences of the old string with the new string.
        /// </summary>
        public void SetAll(string oldStr, string newStr, bool caseSensitive)
        {
            if (oldStr == null) throw new ArgumentNullException(nameof(oldStr), "Old string cannot be null.");
            if (newStr == null) throw new ArgumentNullException(nameof(newStr), "New string cannot be null.");

            for (int i = 0; i < rows; i++)
            {
                rowLocks[i].EnterWriteLock();
                try
                {
                    for (int j = 0; j < cols; j++)
                    {
                        colLocks[j].EnterWriteLock();
                        try
                        {
                            string cellContent = cells[i, j];
                            if ((caseSensitive && cellContent == oldStr) ||
                                (!caseSensitive && string.Equals(cellContent, oldStr, StringComparison.OrdinalIgnoreCase)))
                            {
                                cells[i, j] = newStr;
                            }
                        }
                        finally
                        {
                            colLocks[j].ExitWriteLock();
                        }
                    }
                }
                finally
                {
                    rowLocks[i].ExitWriteLock();
                }
            }
        }

        /// <summary>
        /// Returns the size of the spreadsheet as a tuple of number of rows and columns.
        /// </summary>
        public Tuple<int, int> GetSize()
        {
            return Tuple.Create(rows, cols);
        }

        /// <summary>
        /// Prints the content of the spreadsheet, line by line.
        /// </summary>
        public void Print()
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    Console.Write(cells[i, j] + "\t");
                }
                Console.WriteLine();
            }
        }

        /// <summary>
        /// Validates the specified row and column indices.
        /// </summary>
        private void ValidateIndex(int row, int col)
        {
            if (row < 0 || row >= rows)
                throw new IndexOutOfRangeException($"Row index {row} is out of range.");
            if (col < 0 || col >= cols)
                throw new IndexOutOfRangeException($"Column index {col} is out of range.");
        }

        /// <summary>
        /// Validates the specified row index.
        /// </summary>
        private void ValidateIndex(int row)
        {
            if (row < 0 || row >= rows)
                throw new IndexOutOfRangeException($"Row index {row} is out of range.");
        }

        /// <summary>
        /// Validates the specified column index.
        /// </summary>
        private void ValidateIndex(int col, bool isColumn)
        {
            if (col < 0 || col >= cols)
                throw new IndexOutOfRangeException($"Column index {col} is out of range.");
        }

        /// <summary>
        /// Custom Reader-Writer Lock class for managing access to rows and columns.
        /// </summary>
        private class CustomReaderWriterLock
        {
            private int readerCount = 0;
            private Mutex readerMutex = new Mutex();
            private Semaphore writerSemaphore = new Semaphore(1, 1);

            /// <summary>
            /// Acquires a read lock.
            /// </summary>
            public void EnterReadLock()
            {
                readerMutex.WaitOne();
                readerCount++;
                if (readerCount == 1)
                    writerSemaphore.WaitOne();
                readerMutex.ReleaseMutex();
            }

            /// <summary>
            /// Releases a read lock.
            /// </summary>
            public void ExitReadLock()
            {
                readerMutex.WaitOne();
                readerCount--;
                if (readerCount == 0)
                    writerSemaphore.Release();
                readerMutex.ReleaseMutex();
            }

            /// <summary>
            /// Acquires a write lock.
            /// </summary>
            public void EnterWriteLock()
            {
                writerSemaphore.WaitOne();
            }

            /// <summary>
            /// Releases a write lock.
            /// </summary>
            public void ExitWriteLock()
            {
                writerSemaphore.Release();
            }
        }
    }


