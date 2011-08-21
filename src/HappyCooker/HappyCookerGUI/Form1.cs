using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;

namespace HappyCookerGUI
{
    public partial class Form1 : Form
    {
        private Config m_Config;

        public Form1()
        {
            InitializeComponent();
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
            readConfigFile();
            initConvex();
        }

        private void readConfigFile()
        {
            StreamReader reader = null;

            try { reader = new StreamReader("config.cfg"); }
            catch (FileNotFoundException) { }
            catch (Exception e) { MessageBox.Show(e.Message); }
            m_Config = new Config();

            if (reader != null)
            {
                while (reader.EndOfStream == false)
                {
                    //TODO check for trailing backslash
                    string line = reader.ReadLine();
                    if (line.StartsWith("convexImportPath = "))
                        m_Config.convexImportPath = line.Substring(19);
                    else if (line.StartsWith("convexExportPath = "))
                        m_Config.convexExportPath = line.Substring(19);
                }
                reader.Close();
            }
        }

        private void initConvex()
        {
            m_ConvexImportPath.Text = m_Config.convexImportPath;
            m_ConvexExportPath.Text = m_Config.convexExportPath;

            ListBoxFiller filler = new ListBoxFiller(m_ConvexImportLB, m_Config.convexImportPath, "*.obj",
                                                     m_ConvexConvertLB,
                                                     m_ConvexExportLB, m_Config.convexExportPath, "*.pxconvex");
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            StreamWriter writer = new StreamWriter("config.cfg");

            writer.WriteLine("convexImportPath = " + m_Config.convexImportPath);
            writer.WriteLine("convexExportPath = " + m_Config.convexExportPath);

            writer.Close();
        }

        private void m_ConvexGoBtn_Click(object sender, EventArgs e)
        {
            m_ProgressBar.Maximum = m_ConvexConvertLB.Items.Count + 1;
            m_ProgressBar.Minimum = 0;
            m_ProgressBar.Value = 1;
            m_ProgressBar.Refresh();
            Thread t = new Thread(cookThread);
            t.Start();
        }
        delegate void voidvoid();
        private void IncrementProgressbar()
        {
            if (InvokeRequired)
            {
                Invoke(new voidvoid(IncrementProgressbar));
            }
            else
            {
                m_ProgressBar.Value++;
                m_ProgressBar.Refresh();
            }
        }
        private void ResetProgressbar()
        {
            if (InvokeRequired)
            {
                Invoke(new voidvoid(ResetProgressbar));
            }
            else
            {
                m_ProgressBar.Value = 0;
                m_ProgressBar.Refresh();
            }
        }
        delegate void voidListBox(ListBox lb);
        private void ClearListBox(ListBox lb)
        {
            if (InvokeRequired)
            {
                Invoke(new voidListBox(ClearListBox), lb);
            }
            else
            {
                lb.Items.Clear();
            }
        }
        private void AddItemsListBox(ListBox lb, object[] items)
        {
            if (InvokeRequired)
            {
                Invoke(new voidListBox(ClearListBox), lb);
            }
            else
            {
                lb.Items.AddRange(items);
            }
        }
        private void cookThread()
        {
            HappyCooker cooker = new HappyCooker();
            List<ListboxFile> failList = new List<ListboxFile>();
            List<ListboxFile> successList = new List<ListboxFile>();
            foreach (ListboxFile lbf in m_ConvexConvertLB.Items)
            {
                IncrementProgressbar();
                string outPath = m_Config.convexExportPath +
                    lbf.relativePath.Substring(0, lbf.relativePath.Length - Path.GetExtension(lbf.relativePath).Length) +
                    ".pxconvex";
                string dir = Path.GetDirectoryName(outPath);
                if (Directory.Exists(dir) == false)
                    Directory.CreateDirectory(dir);
                if (cooker.CookObjToConvex(lbf.fullPath, outPath))
                    successList.Add(lbf);
                else
                    failList.Add(lbf);
            }

            cooker.Dispose();

            ClearListBox(m_ConvexConvertLB);
            AddItemsListBox(m_ConvexConvertLB, failList.ToArray());

            Thread.Sleep(1000);
            ResetProgressbar();
        }

        private void m_ConvexImportToExportBtn_Click(object sender, EventArgs e)
        {
            MoveSelectedLbToLb(m_ConvexImportLB, m_ConvexConvertLB);
        }

        private void m_ConvexExportToImportBtn_Click(object sender, EventArgs e)
        {
            MoveSelectedLbToLb(m_ConvexConvertLB, m_ConvexImportLB);
        }

        private void MoveSelectedLbToLb(ListBox from, ListBox to)
        {
            List<object> addedObjects = new List<object>();
            foreach (int i in from.SelectedIndices)
	        {
                to.Items.Add(from.Items[i]);
                addedObjects.Add(from.Items[i]);
	        }
            foreach (object o in addedObjects)
            {
                from.Items.Remove(o);
            }
        }

        private void m_ConvexImportSetPathBtn_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "Select import folder";
            dialog.ShowNewFolderButton = true;
            dialog.SelectedPath = m_Config.convexImportPath;
            if (dialog.ShowDialog() != DialogResult.Cancel)
            {
                m_Config.convexImportPath = dialog.SelectedPath + "\\";
                m_ConvexImportPath.Text = m_Config.convexImportPath;
            }
        }

        private void m_ConvexExportSetPathBtn_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "Select export folder";
            dialog.ShowNewFolderButton = true;
            dialog.SelectedPath = m_Config.convexExportPath;
            if (dialog.ShowDialog() != DialogResult.Cancel)
            {
                m_Config.convexExportPath = dialog.SelectedPath + "\\";
                m_ConvexExportPath.Text = m_Config.convexExportPath;
            }
        }
    }
}
