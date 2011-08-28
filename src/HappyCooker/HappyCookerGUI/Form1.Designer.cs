namespace HappyCookerGUI
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.m_ConvexRefreshBtn = new System.Windows.Forms.Button();
            this.m_ConvexGoBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.m_ConvexConvertToImportBtn = new System.Windows.Forms.Button();
            this.m_ConvexImportToConvertBtn = new System.Windows.Forms.Button();
            this.m_ConvexExportPath = new System.Windows.Forms.TextBox();
            this.m_ConvexImportPath = new System.Windows.Forms.TextBox();
            this.m_ConvexExportSetPathBtn = new System.Windows.Forms.Button();
            this.m_ConvexImportSetPathBtn = new System.Windows.Forms.Button();
            this.m_ConvexConvertLB = new System.Windows.Forms.ListBox();
            this.m_ConvexExportLB = new System.Windows.Forms.ListBox();
            this.m_ConvexImportLB = new System.Windows.Forms.ListBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.m_BinObjRefreshBtn = new System.Windows.Forms.Button();
            this.m_BinObjGoBtn = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.m_BinObjConvertToImportBtn = new System.Windows.Forms.Button();
            this.m_BinObjImportToConvertBtn = new System.Windows.Forms.Button();
            this.m_BinObjExportPath = new System.Windows.Forms.TextBox();
            this.m_BinObjImportPath = new System.Windows.Forms.TextBox();
            this.m_BinObjExportSetPathBtn = new System.Windows.Forms.Button();
            this.m_BinObjImportSetPathBtn = new System.Windows.Forms.Button();
            this.m_BinObjConvertLB = new System.Windows.Forms.ListBox();
            this.m_BinObjExportLB = new System.Windows.Forms.ListBox();
            this.m_BinObjImportLB = new System.Windows.Forms.ListBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.m_ProgressBar = new System.Windows.Forms.ProgressBar();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(966, 582);
            this.tabControl1.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.m_ConvexRefreshBtn);
            this.tabPage1.Controls.Add(this.m_ConvexGoBtn);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.m_ConvexConvertToImportBtn);
            this.tabPage1.Controls.Add(this.m_ConvexImportToConvertBtn);
            this.tabPage1.Controls.Add(this.m_ConvexExportPath);
            this.tabPage1.Controls.Add(this.m_ConvexImportPath);
            this.tabPage1.Controls.Add(this.m_ConvexExportSetPathBtn);
            this.tabPage1.Controls.Add(this.m_ConvexImportSetPathBtn);
            this.tabPage1.Controls.Add(this.m_ConvexConvertLB);
            this.tabPage1.Controls.Add(this.m_ConvexExportLB);
            this.tabPage1.Controls.Add(this.m_ConvexImportLB);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(958, 553);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Convex";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // m_ConvexRefreshBtn
            // 
            this.m_ConvexRefreshBtn.Location = new System.Drawing.Point(300, 236);
            this.m_ConvexRefreshBtn.Name = "m_ConvexRefreshBtn";
            this.m_ConvexRefreshBtn.Size = new System.Drawing.Size(52, 56);
            this.m_ConvexRefreshBtn.TabIndex = 6;
            this.m_ConvexRefreshBtn.Text = "Re-  load";
            this.m_ConvexRefreshBtn.UseVisualStyleBackColor = true;
            // 
            // m_ConvexGoBtn
            // 
            this.m_ConvexGoBtn.Location = new System.Drawing.Point(300, 435);
            this.m_ConvexGoBtn.Name = "m_ConvexGoBtn";
            this.m_ConvexGoBtn.Size = new System.Drawing.Size(52, 76);
            this.m_ConvexGoBtn.TabIndex = 5;
            this.m_ConvexGoBtn.Text = "GO";
            this.m_ConvexGoBtn.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(281, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(396, 31);
            this.label1.TabIndex = 4;
            this.label1.Text = "Obj To Convex Exporter";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // m_ConvexConvertToImportBtn
            // 
            this.m_ConvexConvertToImportBtn.Location = new System.Drawing.Point(300, 131);
            this.m_ConvexConvertToImportBtn.Name = "m_ConvexConvertToImportBtn";
            this.m_ConvexConvertToImportBtn.Size = new System.Drawing.Size(52, 50);
            this.m_ConvexConvertToImportBtn.TabIndex = 3;
            this.m_ConvexConvertToImportBtn.Text = "<<";
            this.m_ConvexConvertToImportBtn.UseVisualStyleBackColor = true;
            // 
            // m_ConvexImportToConvertBtn
            // 
            this.m_ConvexImportToConvertBtn.Location = new System.Drawing.Point(300, 75);
            this.m_ConvexImportToConvertBtn.Name = "m_ConvexImportToConvertBtn";
            this.m_ConvexImportToConvertBtn.Size = new System.Drawing.Size(52, 50);
            this.m_ConvexImportToConvertBtn.TabIndex = 3;
            this.m_ConvexImportToConvertBtn.Text = ">>";
            this.m_ConvexImportToConvertBtn.UseVisualStyleBackColor = true;
            // 
            // m_ConvexExportPath
            // 
            this.m_ConvexExportPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ConvexExportPath.Location = new System.Drawing.Point(664, 43);
            this.m_ConvexExportPath.Name = "m_ConvexExportPath";
            this.m_ConvexExportPath.ReadOnly = true;
            this.m_ConvexExportPath.Size = new System.Drawing.Size(286, 22);
            this.m_ConvexExportPath.TabIndex = 2;
            // 
            // m_ConvexImportPath
            // 
            this.m_ConvexImportPath.Location = new System.Drawing.Point(8, 43);
            this.m_ConvexImportPath.Name = "m_ConvexImportPath";
            this.m_ConvexImportPath.ReadOnly = true;
            this.m_ConvexImportPath.Size = new System.Drawing.Size(286, 22);
            this.m_ConvexImportPath.TabIndex = 2;
            // 
            // m_ConvexExportSetPathBtn
            // 
            this.m_ConvexExportSetPathBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ConvexExportSetPathBtn.Location = new System.Drawing.Point(836, 6);
            this.m_ConvexExportSetPathBtn.Name = "m_ConvexExportSetPathBtn";
            this.m_ConvexExportSetPathBtn.Size = new System.Drawing.Size(114, 31);
            this.m_ConvexExportSetPathBtn.TabIndex = 1;
            this.m_ConvexExportSetPathBtn.Text = "Set Export Path";
            this.m_ConvexExportSetPathBtn.UseVisualStyleBackColor = true;
            // 
            // m_ConvexImportSetPathBtn
            // 
            this.m_ConvexImportSetPathBtn.Location = new System.Drawing.Point(8, 6);
            this.m_ConvexImportSetPathBtn.Name = "m_ConvexImportSetPathBtn";
            this.m_ConvexImportSetPathBtn.Size = new System.Drawing.Size(114, 31);
            this.m_ConvexImportSetPathBtn.TabIndex = 1;
            this.m_ConvexImportSetPathBtn.Text = "Set Import Path";
            this.m_ConvexImportSetPathBtn.UseVisualStyleBackColor = true;
            // 
            // m_ConvexConvertLB
            // 
            this.m_ConvexConvertLB.FormattingEnabled = true;
            this.m_ConvexConvertLB.ItemHeight = 16;
            this.m_ConvexConvertLB.Location = new System.Drawing.Point(358, 75);
            this.m_ConvexConvertLB.Name = "m_ConvexConvertLB";
            this.m_ConvexConvertLB.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.m_ConvexConvertLB.Size = new System.Drawing.Size(286, 436);
            this.m_ConvexConvertLB.Sorted = true;
            this.m_ConvexConvertLB.TabIndex = 0;
            // 
            // m_ConvexExportLB
            // 
            this.m_ConvexExportLB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ConvexExportLB.FormattingEnabled = true;
            this.m_ConvexExportLB.ItemHeight = 16;
            this.m_ConvexExportLB.Location = new System.Drawing.Point(664, 75);
            this.m_ConvexExportLB.Name = "m_ConvexExportLB";
            this.m_ConvexExportLB.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.m_ConvexExportLB.Size = new System.Drawing.Size(286, 436);
            this.m_ConvexExportLB.Sorted = true;
            this.m_ConvexExportLB.TabIndex = 0;
            // 
            // m_ConvexImportLB
            // 
            this.m_ConvexImportLB.FormattingEnabled = true;
            this.m_ConvexImportLB.ItemHeight = 16;
            this.m_ConvexImportLB.Location = new System.Drawing.Point(8, 75);
            this.m_ConvexImportLB.Name = "m_ConvexImportLB";
            this.m_ConvexImportLB.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.m_ConvexImportLB.Size = new System.Drawing.Size(286, 436);
            this.m_ConvexImportLB.Sorted = true;
            this.m_ConvexImportLB.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.m_BinObjRefreshBtn);
            this.tabPage2.Controls.Add(this.m_BinObjGoBtn);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.m_BinObjConvertToImportBtn);
            this.tabPage2.Controls.Add(this.m_BinObjImportToConvertBtn);
            this.tabPage2.Controls.Add(this.m_BinObjExportPath);
            this.tabPage2.Controls.Add(this.m_BinObjImportPath);
            this.tabPage2.Controls.Add(this.m_BinObjExportSetPathBtn);
            this.tabPage2.Controls.Add(this.m_BinObjImportSetPathBtn);
            this.tabPage2.Controls.Add(this.m_BinObjConvertLB);
            this.tabPage2.Controls.Add(this.m_BinObjExportLB);
            this.tabPage2.Controls.Add(this.m_BinObjImportLB);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(958, 553);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "BinObj";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // m_BinObjRefreshBtn
            // 
            this.m_BinObjRefreshBtn.Location = new System.Drawing.Point(300, 236);
            this.m_BinObjRefreshBtn.Name = "m_BinObjRefreshBtn";
            this.m_BinObjRefreshBtn.Size = new System.Drawing.Size(52, 56);
            this.m_BinObjRefreshBtn.TabIndex = 18;
            this.m_BinObjRefreshBtn.Text = "Re-  load";
            this.m_BinObjRefreshBtn.UseVisualStyleBackColor = true;
            // 
            // m_BinObjGoBtn
            // 
            this.m_BinObjGoBtn.Location = new System.Drawing.Point(300, 435);
            this.m_BinObjGoBtn.Name = "m_BinObjGoBtn";
            this.m_BinObjGoBtn.Size = new System.Drawing.Size(52, 76);
            this.m_BinObjGoBtn.TabIndex = 17;
            this.m_BinObjGoBtn.Text = "GO";
            this.m_BinObjGoBtn.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(281, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(396, 31);
            this.label2.TabIndex = 16;
            this.label2.Text = "Obj To BinObj Exporter";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // m_BinObjConvertToImportBtn
            // 
            this.m_BinObjConvertToImportBtn.Location = new System.Drawing.Point(300, 131);
            this.m_BinObjConvertToImportBtn.Name = "m_BinObjConvertToImportBtn";
            this.m_BinObjConvertToImportBtn.Size = new System.Drawing.Size(52, 50);
            this.m_BinObjConvertToImportBtn.TabIndex = 14;
            this.m_BinObjConvertToImportBtn.Text = "<<";
            this.m_BinObjConvertToImportBtn.UseVisualStyleBackColor = true;
            // 
            // m_BinObjImportToConvertBtn
            // 
            this.m_BinObjImportToConvertBtn.Location = new System.Drawing.Point(300, 75);
            this.m_BinObjImportToConvertBtn.Name = "m_BinObjImportToConvertBtn";
            this.m_BinObjImportToConvertBtn.Size = new System.Drawing.Size(52, 50);
            this.m_BinObjImportToConvertBtn.TabIndex = 15;
            this.m_BinObjImportToConvertBtn.Text = ">>";
            this.m_BinObjImportToConvertBtn.UseVisualStyleBackColor = true;
            // 
            // m_BinObjExportPath
            // 
            this.m_BinObjExportPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_BinObjExportPath.Location = new System.Drawing.Point(664, 43);
            this.m_BinObjExportPath.Name = "m_BinObjExportPath";
            this.m_BinObjExportPath.ReadOnly = true;
            this.m_BinObjExportPath.Size = new System.Drawing.Size(286, 22);
            this.m_BinObjExportPath.TabIndex = 13;
            // 
            // m_BinObjImportPath
            // 
            this.m_BinObjImportPath.Location = new System.Drawing.Point(8, 43);
            this.m_BinObjImportPath.Name = "m_BinObjImportPath";
            this.m_BinObjImportPath.ReadOnly = true;
            this.m_BinObjImportPath.Size = new System.Drawing.Size(286, 22);
            this.m_BinObjImportPath.TabIndex = 12;
            // 
            // m_BinObjExportSetPathBtn
            // 
            this.m_BinObjExportSetPathBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_BinObjExportSetPathBtn.Location = new System.Drawing.Point(836, 6);
            this.m_BinObjExportSetPathBtn.Name = "m_BinObjExportSetPathBtn";
            this.m_BinObjExportSetPathBtn.Size = new System.Drawing.Size(114, 31);
            this.m_BinObjExportSetPathBtn.TabIndex = 10;
            this.m_BinObjExportSetPathBtn.Text = "Set Export Path";
            this.m_BinObjExportSetPathBtn.UseVisualStyleBackColor = true;
            // 
            // m_BinObjImportSetPathBtn
            // 
            this.m_BinObjImportSetPathBtn.Location = new System.Drawing.Point(8, 6);
            this.m_BinObjImportSetPathBtn.Name = "m_BinObjImportSetPathBtn";
            this.m_BinObjImportSetPathBtn.Size = new System.Drawing.Size(114, 31);
            this.m_BinObjImportSetPathBtn.TabIndex = 11;
            this.m_BinObjImportSetPathBtn.Text = "Set Import Path";
            this.m_BinObjImportSetPathBtn.UseVisualStyleBackColor = true;
            // 
            // m_BinObjConvertLB
            // 
            this.m_BinObjConvertLB.FormattingEnabled = true;
            this.m_BinObjConvertLB.ItemHeight = 16;
            this.m_BinObjConvertLB.Location = new System.Drawing.Point(358, 75);
            this.m_BinObjConvertLB.Name = "m_BinObjConvertLB";
            this.m_BinObjConvertLB.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.m_BinObjConvertLB.Size = new System.Drawing.Size(286, 436);
            this.m_BinObjConvertLB.Sorted = true;
            this.m_BinObjConvertLB.TabIndex = 7;
            // 
            // m_BinObjExportLB
            // 
            this.m_BinObjExportLB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_BinObjExportLB.FormattingEnabled = true;
            this.m_BinObjExportLB.ItemHeight = 16;
            this.m_BinObjExportLB.Location = new System.Drawing.Point(664, 75);
            this.m_BinObjExportLB.Name = "m_BinObjExportLB";
            this.m_BinObjExportLB.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.m_BinObjExportLB.Size = new System.Drawing.Size(286, 436);
            this.m_BinObjExportLB.Sorted = true;
            this.m_BinObjExportLB.TabIndex = 8;
            // 
            // m_BinObjImportLB
            // 
            this.m_BinObjImportLB.FormattingEnabled = true;
            this.m_BinObjImportLB.ItemHeight = 16;
            this.m_BinObjImportLB.Location = new System.Drawing.Point(8, 75);
            this.m_BinObjImportLB.Name = "m_BinObjImportLB";
            this.m_BinObjImportLB.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.m_BinObjImportLB.Size = new System.Drawing.Size(286, 436);
            this.m_BinObjImportLB.Sorted = true;
            this.m_BinObjImportLB.TabIndex = 9;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.m_ProgressBar);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 549);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(966, 33);
            this.panel1.TabIndex = 3;
            // 
            // m_ProgressBar
            // 
            this.m_ProgressBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ProgressBar.Location = new System.Drawing.Point(3, 5);
            this.m_ProgressBar.Name = "m_ProgressBar";
            this.m_ProgressBar.Size = new System.Drawing.Size(960, 23);
            this.m_ProgressBar.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(966, 582);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "HappyCooker";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button m_ConvexGoBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button m_ConvexConvertToImportBtn;
        private System.Windows.Forms.Button m_ConvexImportToConvertBtn;
        private System.Windows.Forms.TextBox m_ConvexExportPath;
        private System.Windows.Forms.TextBox m_ConvexImportPath;
        private System.Windows.Forms.Button m_ConvexExportSetPathBtn;
        private System.Windows.Forms.Button m_ConvexImportSetPathBtn;
        private System.Windows.Forms.ListBox m_ConvexExportLB;
        private System.Windows.Forms.ListBox m_ConvexImportLB;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ProgressBar m_ProgressBar;
        private System.Windows.Forms.ListBox m_ConvexConvertLB;
        private System.Windows.Forms.Button m_ConvexRefreshBtn;
        private System.Windows.Forms.Button m_BinObjRefreshBtn;
        private System.Windows.Forms.Button m_BinObjGoBtn;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button m_BinObjConvertToImportBtn;
        private System.Windows.Forms.Button m_BinObjImportToConvertBtn;
        private System.Windows.Forms.TextBox m_BinObjExportPath;
        private System.Windows.Forms.TextBox m_BinObjImportPath;
        private System.Windows.Forms.Button m_BinObjExportSetPathBtn;
        private System.Windows.Forms.Button m_BinObjImportSetPathBtn;
        private System.Windows.Forms.ListBox m_BinObjConvertLB;
        private System.Windows.Forms.ListBox m_BinObjExportLB;
        private System.Windows.Forms.ListBox m_BinObjImportLB;
    }
}

