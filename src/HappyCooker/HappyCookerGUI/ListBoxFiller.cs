using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace HappyCookerGUI
{
    class ListBoxFiller
    {
        private ListBox m_Import, m_Export, m_Convert;
        private string m_ImportPath, m_ExportPath;
        private string m_ImportFilter, m_ExportFilter;
        
        public ListBoxFiller(ListBox import, string importPath, string importFilter,
                             ListBox convert,
                             ListBox export, string exportPath, string exportFilter)
        {
            m_Import = import;
            m_ImportPath = importPath;
            m_ImportFilter = importFilter;

            m_Export = export;
            m_ExportPath = exportPath;
            m_ExportFilter = exportFilter;

            m_Convert = convert;

            AddExportFiles();
            AddImportFiles();
        }

        private void AddExportFiles()
        {
            try
            {
                IEnumerable<string> files = Directory.EnumerateFiles(m_ExportPath, m_ExportFilter, SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    ListboxFile lbf = new ListboxFile();
                    lbf.fullPath = file;
                    lbf.relativePath = file.Substring(m_ExportPath.Length);
                    m_Export.Items.Add(lbf);
                }
            }
            catch (Exception) { }
        }
        private void AddImportFiles()
        {
            try
            {
                IEnumerable<string> files = Directory.EnumerateFiles(m_ImportPath, m_ImportFilter, SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    ListboxFile lbf = new ListboxFile();
                    lbf.fullPath = file;
                    lbf.relativePath = file.Substring(m_ImportPath.Length);

                    if (needsUpdate(lbf))
                        m_Convert.Items.Add(lbf);
                    else
                        m_Import.Items.Add(lbf);
                }
            }
            catch (Exception) { }
        }
        private bool needsUpdate(ListboxFile importFile)
        {
            string importFileName = importFile.relativePath.Substring(0, 
                importFile.relativePath.Length - Path.GetExtension(importFile.relativePath).Length);
            foreach (ListboxFile lbf in m_Export.Items)
            {
                string exportFileName = lbf.relativePath.Substring(0,
                    lbf.relativePath.Length - Path.GetExtension(lbf.relativePath).Length);
                if (exportFileName == importFileName)
                {
                    if (File.GetLastWriteTime(importFile.fullPath) > File.GetLastWriteTime(lbf.fullPath))
                        return true;
                    else
                        return false;
                }
            }
            return true;
        }
    }
}
