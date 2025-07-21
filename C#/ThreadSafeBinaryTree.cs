using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public interface IBinaryTree
{
    void Add(string value);
    void Delete(string value);
    int Search(string value);
    void PrintSorted();
}

public class ThreadSafeBinaryTree : IBinaryTree
{
    private class Node
    {
        public string Value { get; set; }
        public int ReferenceCount { get; set; }
        public Node Left { get; set; }
        public Node Right { get; set; }

        public Node(string value)
        {
            Value = value;
            ReferenceCount = 1;
            Left = null;
            Right = null;
        }
    }

    private Node root;
    private Mutex mutex = new Mutex();
    private Semaphore semaphore = new Semaphore(10, 10); // Allow up to 10 concurrent readers

    public void Add(string value)
    {
        mutex.WaitOne(); // Ensure exclusive access for adding
        try
        {
            root = AddRecursive(root, value);
        }
        finally
        {
            mutex.ReleaseMutex(); // Release the mutex
        }
    }

    private Node AddRecursive(Node node, string value)
    {
        if (node == null)
        {
            return new Node(value);
        }

        int comparison = string.Compare(value, node.Value, StringComparison.Ordinal);
        if (comparison < 0)
        {
            node.Left = AddRecursive(node.Left, value);
        }
        else if (comparison > 0)
        {
            node.Right = AddRecursive(node.Right, value);
        }
        else
        {
            node.ReferenceCount++;  // Increment the reference count if the value already exists
        }

        return node;
    }

    public void Delete(string value)
    {
        mutex.WaitOne(); // Ensure exclusive access for deletion
        try
        {
            root = DeleteRecursive(root, value);
        }
        finally
        {
            mutex.ReleaseMutex(); // Release the mutex
        }
    }

    private Node DeleteRecursive(Node node, string value)
    {
        if (node == null) return null;

        int comparison = string.Compare(value, node.Value, StringComparison.Ordinal);
        if (comparison < 0)
        {
            node.Left = DeleteRecursive(node.Left, value);
        }
        else if (comparison > 0)
        {
            node.Right = DeleteRecursive(node.Right, value);
        }
        else
        {
            // If we found the node
            node.ReferenceCount--;  // Decrement the reference count
            if (node.ReferenceCount <= 0)
            {
                // Node should be deleted
                if (node.Left == null) return node.Right;
                if (node.Right == null) return node.Left;

                // Node with two children: Get the inorder successor (smallest in the right subtree)
                Node minNode = GetMinNode(node.Right);
                node.Value = minNode.Value;
                node.ReferenceCount = minNode.ReferenceCount;
                node.Right = DeleteRecursive(node.Right, minNode.Value);
            }
        }

        return node;
    }

    private Node GetMinNode(Node node)
    {
        while (node.Left != null)
        {
            node = node.Left;
        }
        return node;
    }

    public int Search(string value)
    {
        semaphore.WaitOne(); // Allow multiple concurrent readers
        try
        {
            return SearchRecursive(root, value);
        }
        finally
        {
            semaphore.Release(); // Release the semaphore
        }
    }

    private int SearchRecursive(Node node, string value)
    {
        if (node == null) return 0;

        int comparison = string.Compare(value, node.Value, StringComparison.Ordinal);
        if (comparison < 0)
        {
            return SearchRecursive(node.Left, value);
        }
        else if (comparison > 0)
        {
            return SearchRecursive(node.Right, value);
        }
        else
        {
            return node.ReferenceCount;  // Return the reference count
        }
    }

    public void PrintSorted()
    {
        semaphore.WaitOne(); // Allow multiple concurrent prints
        try
        {
            PrintSortedRecursive(root);
        }
        finally
        {
            semaphore.Release(); // Release the semaphore
        }
    }

    private void PrintSortedRecursive(Node node)
    {
        if (node != null)
        {
            PrintSortedRecursive(node.Left);
            Console.WriteLine($"{node.Value} ({node.ReferenceCount})");
            PrintSortedRecursive(node.Right);
        }
    }
}
