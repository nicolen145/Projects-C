using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;

namespace DDoSAttack
{
    public partial class Form1 : Form
    {
        // List to keep track of all started processes
        private List<Process> processes = new List<Process>();

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            // Validate the input for the number of processes
            if (!int.TryParse(textBox1.Text, out int count) || count <= 0)
            {
                MessageBox.Show("Please enter a valid number of processes to start.");
                return;
            }

            // Validate the URL or executable path input
            string url = textBox2.Text;
            if (string.IsNullOrWhiteSpace(url))
            {
                MessageBox.Show("Please enter a valid URL or executable path.");
                return;
            }

            // Start the specified number of processes with the given URL
            StartProcesses(url, count);
        }

        private void StartProcesses(string url, int count)
        {
            for (int i = 0; i < count; i++)
            {
                try
                {
                    // Start a new process with the specified URL
                    var process = Process.Start(new ProcessStartInfo
                    {
                        FileName = url,
                        Arguments = $"--new-window {url}", // Pass the URL as an argument
                        UseShellExecute = true
                    });

                    // Add the process to the list if it was started successfully
                    if (process != null)
                    {
                        processes.Add(process);
                    }
                }
                catch (Exception ex)
                {
                    // Display an error message if starting the process fails
                    MessageBox.Show($"Error starting process {i + 1}: {ex.Message}");
                }

                // Delay between starting processes to avoid overwhelming the system
                System.Threading.Thread.Sleep(300);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Close all the processes that were started
            CloseProcesses(textBox2.Text);
        }

        private void CloseProcesses(string url)
        {
            foreach (var process in processes.ToList())
            {
                try
                {
                    if (process != null && !process.HasExited)
                    {
                        // Check if the process arguments contain the URL
                        var processArguments = process.StartInfo.Arguments;
                        if (processArguments.Contains(url))
                        {
                            process.Kill(); // Terminate the process
                            process.WaitForExit(); // Ensure the process has exited
                        }
                    }
                }
                catch (Exception ex)
                {
                    // Display an error message if closing the process fails
                    MessageBox.Show($"Error closing browser process {process.Id}: {ex.Message}");
                }
            }

            // Clear the list of processes after closing them
            processes.Clear();

            // Inform the user that all browsers have been closed
            MessageBox.Show("All browsers have been closed.");
        }

        // Event handlers for form load and control events (currently empty)
        private void Form1_Load(object sender, EventArgs e) { }

        private void label1_Click(object sender, EventArgs e) { }

        private void label2_Click(object sender, EventArgs e) { }

        private void textBox1_TextChanged(object sender, EventArgs e) { }

        private void textBox2_TextChanged(object sender, EventArgs e) { }
    }
}
