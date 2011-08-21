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
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.m_ConvexImportLB = new System.Windows.Forms.ListBox();
            this.m_ConvexImportSetPathBtn = new System.Windows.Forms.Button();
            this.m_ConvexImportPath = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.m_ProgressBar = new System.Windows.Forms.ProgressBar();
            this.m_ConvexExportLB = new System.Windows.Forms.ListBox();
            this.m_ConvexExportPath = new System.Windows.Forms.TextBox();
            this.m_ConvexExportSetPathBtn = new System.Windows.Forms.Button();
            this.m_ConvexImportToExportBtn = new System.Windows.Forms.Button();
            this.m_ConvexExportToImportBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.m_ConvexGoBtn = new System.Windows.Forms.Button();
            this.m_ConvexConvertLB = new System.Windows.Forms.ListBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
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
            this.tabPage1.Controls.Add(this.m_ConvexGoBtn);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.m_ConvexExportToImportBtn);
            this.tabPage1.Controls.Add(this.m_ConvexImportToExportBtn);
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
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1075, 553);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
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
            // m_ConvexImportSetPathBtn
            // 
            this.m_ConvexImportSetPathBtn.Location = new System.Drawing.Point(8, 6);
            this.m_ConvexImportSetPathBtn.Name = "m_ConvexImportSetPathBtn";
            this.m_ConvexImportSetPathBtn.Size = new System.Drawing.Size(114, 31);
            this.m_ConvexImportSetPathBtn.TabIndex = 1;
            this.m_ConvexImportSetPathBtn.Text = "Set Import Path";
            this.m_ConvexImportSetPathBtn.UseVisualStyleBackColor = true;
            this.m_ConvexImportSetPathBtn.Click += new System.EventHandler(this.m_ConvexImportSetPathBtn_Click);
            // 
            // m_ConvexImportPath
            // 
            this.m_ConvexImportPath.Location = new System.Drawing.Point(8, 43);
            this.m_ConvexImportPath.Name = "m_ConvexImportPath";
            this.m_ConvexImportPath.ReadOnly = true;
            this.m_ConvexImportPath.Size = new System.Drawing.Size(286, 22);
            this.m_ConvexImportPath.TabIndex = 2;
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
            // m_ConvexExportPath
            // 
            this.m_ConvexExportPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ConvexExportPath.Location = new System.Drawing.Point(664, 43);
            this.m_ConvexExportPath.Name = "m_ConvexExportPath";
            this.m_ConvexExportPath.ReadOnly = true;
            this.m_ConvexExportPath.Size = new System.Drawing.Size(286, 22);
            this.m_ConvexExportPath.TabIndex = 2;
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
            this.m_ConvexExportSetPathBtn.Click += new System.EventHandler(this.m_ConvexExportSetPathBtn_Click);
            // 
            // m_ConvexImportToExportBtn
            // 
            this.m_ConvexImportToExportBtn.Location = new System.Drawing.Point(300, 75);
            this.m_ConvexImportToExportBtn.Name = "m_ConvexImportToExportBtn";
            this.m_ConvexImportToExportBtn.Size = new System.Drawing.Size(52, 50);
            this.m_ConvexImportToExportBtn.TabIndex = 3;
            this.m_ConvexImportToExportBtn.Text = ">>";
            this.m_ConvexImportToExportBtn.UseVisualStyleBackColor = true;
            this.m_ConvexImportToExportBtn.Click += new System.EventHandler(this.m_ConvexImportToExportBtn_Click);
            // 
            // m_ConvexExportToImportBtn
            // 
            this.m_ConvexExportToImportBtn.Location = new System.Drawing.Point(300, 131);
            this.m_ConvexExportToImportBtn.Name = "m_ConvexExportToImportBtn";
            this.m_ConvexExportToImportBtn.Size = new System.Drawing.Size(52, 50);
            this.m_ConvexExportToImportBtn.TabIndex = 3;
            this.m_ConvexExportToImportBtn.Text = "<<";
            this.m_ConvexExportToImportBtn.UseVisualStyleBackColor = true;
            this.m_ConvexExportToImportBtn.Click += new System.EventHandler(this.m_ConvexExportToImportBtn_Click);
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
            // m_ConvexGoBtn
            // 
            this.m_ConvexGoBtn.Location = new System.Drawing.Point(300, 435);
            this.m_ConvexGoBtn.Name = "m_ConvexGoBtn";
            this.m_ConvexGoBtn.Size = new System.Drawing.Size(52, 76);
            this.m_ConvexGoBtn.TabIndex = 5;
            this.m_ConvexGoBtn.Text = "GO";
            this.m_ConvexGoBtn.UseVisualStyleBackColor = true;
            this.m_ConvexGoBtn.Click += new System.EventHandler(this.m_ConvexGoBtn_Click);
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
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button m_ConvexGoBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button m_ConvexExportToImportBtn;
        private System.Windows.Forms.Button m_ConvexImportToExportBtn;
        private System.Windows.Forms.TextBox m_ConvexExportPath;
        private System.Windows.Forms.TextBox m_ConvexImportPath;
        private System.Windows.Forms.Button m_ConvexExportSetPathBtn;
        private System.Windows.Forms.Button m_ConvexImportSetPathBtn;
        private System.Windows.Forms.ListBox m_ConvexExportLB;
        private System.Windows.Forms.ListBox m_ConvexImportLB;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ProgressBar m_ProgressBar;
        private System.Windows.Forms.ListBox m_ConvexConvertLB;
    }
}

