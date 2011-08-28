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

        private const int COOKERS = 2;
        private Button[] m_ImportPathBtn = new Button[COOKERS];
        private Button[] m_ExportPathBtn = new Button[COOKERS];
        private TextBox[] m_ImportPath = new TextBox[COOKERS];
        private TextBox[] m_ExportPath = new TextBox[COOKERS];
        private ListBox[] m_ImportListbox = new ListBox[COOKERS];
        private ListBox[] m_ConvertListbox = new ListBox[COOKERS];
        private ListBox[] m_ExportListbox = new ListBox[COOKERS];
        private Button[] m_ImportToConvertBtn = new Button[COOKERS];
        private Button[] m_ConvertToImportBtn = new Button[COOKERS];
        private Button[] m_GoBtn = new Button[COOKERS];

        private string[] m_CookerExtension = new string[COOKERS] { ".pxconvex", ".binobj" };

        private int m_CurrentCooker = 0;

        public Form1()
        {
            InitializeComponent();

            #region Convex
            m_ImportPathBtn[0] = m_ConvexImportSetPathBtn;
            m_ExportPathBtn[0] = m_ConvexExportSetPathBtn;
            m_ImportPath[0] = m_ConvexImportPath;
            m_ExportPath[0] = m_ConvexExportPath;
            m_ImportListbox[0] = m_ConvexImportLB;
            m_ConvertListbox[0] = m_ConvexConvertLB;
            m_ExportListbox[0] = m_ConvexExportLB;
            m_ImportToConvertBtn[0] = m_ConvexImportToConvertBtn;
            m_ConvertToImportBtn[0] = m_ConvexConvertToImportBtn;
            m_GoBtn[0] = m_ConvexGoBtn;
            #endregion
            #region BinObj
            m_ImportPathBtn[1] = m_BinObjImportSetPathBtn;
            m_ExportPathBtn[1] = m_BinObjExportSetPathBtn;
            m_ImportPath[1] = m_BinObjImportPath;
            m_ExportPath[1] = m_BinObjExportPath;
            m_ImportListbox[1] = m_BinObjImportLB;
            m_ConvertListbox[1] = m_BinObjConvertLB;
            m_ExportListbox[1] = m_BinObjExportLB;
            m_ImportToConvertBtn[1] = m_BinObjImportToConvertBtn;
            m_ConvertToImportBtn[1] = m_BinObjConvertToImportBtn;
            m_GoBtn[1] = m_BinObjGoBtn;
            #endregion

            for (int i = 0; i < COOKERS; i++)
            {
                m_ImportPathBtn[i].Tag = i;
                m_ImportPathBtn[i].Click += importSetPathBtn_Click;

                m_ExportPathBtn[i].Tag = i;
                m_ExportPathBtn[i].Click += exportSetPathBtn_Click;

                m_ImportToConvertBtn[i].Tag = i;
                m_ImportToConvertBtn[i].Click += importToConvertBtn_Click;

                m_ConvertToImportBtn[i].Tag = i;
                m_ConvertToImportBtn[i].Click += convertToImportBtn_Click;

                m_GoBtn[i].Tag = i;
                m_GoBtn[i].Click += goBtn_Click;
            }
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
            readConfigFile();
            init();
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
                    else if (line.StartsWith("binobjImportPath = "))
                        m_Config.binobjImportPath = line.Substring(19);
                    else if (line.StartsWith("binobjExportPath = "))
                        m_Config.binobjExportPath = line.Substring(19);
                }
                reader.Close();
            }
        }

        private void init()
        {
            initConvex();
            initBinObj();
        }
        private void initConvex()
        {
            m_ImportPath[0].Text = m_Config.convexImportPath;
            m_ExportPath[0].Text = m_Config.convexExportPath;

            ListBoxFiller filler = new ListBoxFiller(m_ImportListbox[0], m_Config.convexImportPath, "*.obj",
                                                     m_ConvertListbox[0],
                                                     m_ExportListbox[0], m_Config.convexExportPath, "*.pxconvex");
        }
        private void initBinObj()
        {
            m_ImportPath[1].Text = m_Config.binobjImportPath;
            m_ExportPath[1].Text = m_Config.binobjExportPath;

            ListBoxFiller filler = new ListBoxFiller(m_ImportListbox[1], m_Config.binobjImportPath, "*.obj",
                                                     m_ConvertListbox[1],
                                                     m_ExportListbox[1], m_Config.binobjExportPath, "*.binobj");
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            StreamWriter writer = new StreamWriter("config.cfg");

            writer.WriteLine("convexImportPath = " + m_ImportPath[0].Text);
            writer.WriteLine("convexExportPath = " + m_ExportPath[0].Text);
            writer.WriteLine("binobjImportPath = " + m_ImportPath[1].Text);
            writer.WriteLine("binobjExportPath = " + m_ExportPath[1].Text);

            writer.Close();
        }

        private void goBtn_Click(object sender, EventArgs e)
        {
            m_CurrentCooker = (int)(sender as Button).Tag;
            m_ProgressBar.Maximum = m_ConvertListbox[m_CurrentCooker].Items.Count + 1;
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
            foreach (ListboxFile lbf in m_ConvertListbox[m_CurrentCooker].Items)
            {
                IncrementProgressbar();
                string outPath = m_ExportPath[m_CurrentCooker].Text +
                    lbf.relativePath.Substring(0, lbf.relativePath.Length - Path.GetExtension(lbf.relativePath).Length) +
                    m_CookerExtension[m_CurrentCooker];
                string dir = Path.GetDirectoryName(outPath);
                if (Directory.Exists(dir) == false)
                    Directory.CreateDirectory(dir);

                bool success = false;
                switch (m_CurrentCooker)
                {
                    case 0: success = cooker.CookObjToConvex(lbf.fullPath, outPath); break;
                    case 1: success = cooker.CookObjToBinObj(lbf.fullPath, outPath); break;
                }
                if (success)
                    successList.Add(lbf);
                else
                    failList.Add(lbf);
            }

            cooker.Dispose();

            ClearListBox(m_ConvertListbox[m_CurrentCooker]);
            AddItemsListBox(m_ConvertListbox[m_CurrentCooker], failList.ToArray());

            Thread.Sleep(1000);
            ResetProgressbar();
        }

        private void importToConvertBtn_Click(object sender, EventArgs e)
        {
            Button b = sender as Button;
            int cooker = (int)b.Tag;
            MoveSelectedLbToLb(m_ImportListbox[cooker], m_ConvertListbox[cooker]);
        }

        private void convertToImportBtn_Click(object sender, EventArgs e)
        {
            Button b = sender as Button;
            int cooker = (int)b.Tag;
            MoveSelectedLbToLb(m_ConvertListbox[cooker], m_ImportListbox[cooker]);
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

        private void importSetPathBtn_Click(object sender, EventArgs e)
        {
            Button b = sender as Button;
            int cooker = (int)b.Tag;

            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "Select import folder";
            dialog.ShowNewFolderButton = true;
            dialog.SelectedPath = m_ImportPath[cooker].Text;
            if (dialog.ShowDialog() != DialogResult.Cancel)
            {
                m_ImportPath[cooker].Text = dialog.SelectedPath + "\\";
            }
        }

        private void exportSetPathBtn_Click(object sender, EventArgs e)
        {
            Button b = sender as Button;
            int cooker = (int)b.Tag;

            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "Select export folder";
            dialog.ShowNewFolderButton = true;
            dialog.SelectedPath = m_ExportPath[cooker].Text;
            if (dialog.ShowDialog() != DialogResult.Cancel)
            {
                m_ExportPath[cooker].Text = dialog.SelectedPath + "\\";
            }
        }
    }
}
