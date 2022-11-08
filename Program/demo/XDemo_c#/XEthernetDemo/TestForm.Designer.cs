namespace XEthernetDemo
{
    partial class TestForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TestForm));
            this.FindDeviceButton = new System.Windows.Forms.Button();
            this.StartButton = new System.Windows.Forms.Button();
            this.StopButton = new System.Windows.Forms.Button();
            this.LostLine = new System.Windows.Forms.Label();
            this.Error = new System.Windows.Forms.Label();
            this.Humidity = new System.Windows.Forms.Label();
            this.Temperature = new System.Windows.Forms.Label();
            this.SN = new System.Windows.Forms.Label();
            this.DisWin = new System.Windows.Forms.PictureBox();
            this.ChannelChecktimer = new System.Windows.Forms.Timer(this.components);
            this.FunctionBox = new System.Windows.Forms.GroupBox();
            this.SingleChannelEnableCheckBox = new System.Windows.Forms.CheckBox();
            this.ChannelListComboBox = new System.Windows.Forms.ComboBox();
            this.FunctionSelect_YelCu = new System.Windows.Forms.CheckBox();
            this.FunctionSelect_NoSelect = new System.Windows.Forms.CheckBox();
            this.FunctionSelect_Fe = new System.Windows.Forms.CheckBox();
            this.FunctionSelect_Pb = new System.Windows.Forms.CheckBox();
            this.FunctionSelect_Zn = new System.Windows.Forms.CheckBox();
            this.FunctionSelect_Cu = new System.Windows.Forms.CheckBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.Version = new System.Windows.Forms.TextBox();
            this.paraSetButton = new System.Windows.Forms.Button();
            this.refreshThreshButton = new System.Windows.Forms.Button();
            this.PowerPanelEnableButton = new System.Windows.Forms.Button();
            this.CMOSCheckBox = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.DisWin)).BeginInit();
            this.FunctionBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // FindDeviceButton
            // 
            this.FindDeviceButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.FindDeviceButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FindDeviceButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FindDeviceButton.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.FindDeviceButton.Location = new System.Drawing.Point(737, 217);
            this.FindDeviceButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.FindDeviceButton.Name = "FindDeviceButton";
            this.FindDeviceButton.Size = new System.Drawing.Size(136, 61);
            this.FindDeviceButton.TabIndex = 3;
            this.FindDeviceButton.Tag = "0";
            this.FindDeviceButton.Text = "查找设备";
            this.FindDeviceButton.UseVisualStyleBackColor = false;
            this.FindDeviceButton.Click += new System.EventHandler(this.FindDeviceButton_Click);
            // 
            // StartButton
            // 
            this.StartButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.StartButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.StartButton.Enabled = false;
            this.StartButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.StartButton.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.StartButton.Location = new System.Drawing.Point(737, 317);
            this.StartButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(136, 61);
            this.StartButton.TabIndex = 4;
            this.StartButton.Tag = "0";
            this.StartButton.Text = "启动";
            this.StartButton.UseVisualStyleBackColor = false;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            // 
            // StopButton
            // 
            this.StopButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.StopButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.StopButton.Enabled = false;
            this.StopButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.StopButton.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.StopButton.Location = new System.Drawing.Point(737, 417);
            this.StopButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StopButton.Name = "StopButton";
            this.StopButton.Size = new System.Drawing.Size(136, 61);
            this.StopButton.TabIndex = 5;
            this.StopButton.Tag = "0";
            this.StopButton.Text = "停止";
            this.StopButton.UseVisualStyleBackColor = false;
            this.StopButton.Click += new System.EventHandler(this.StopButton_Click);
            // 
            // LostLine
            // 
            this.LostLine.AutoSize = true;
            this.LostLine.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.LostLine.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.LostLine.Location = new System.Drawing.Point(33, 45);
            this.LostLine.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.LostLine.Name = "LostLine";
            this.LostLine.Size = new System.Drawing.Size(71, 17);
            this.LostLine.TabIndex = 12;
            this.LostLine.Text = "Lost Line: ";
            // 
            // Error
            // 
            this.Error.AutoSize = true;
            this.Error.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Error.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Error.Location = new System.Drawing.Point(368, 45);
            this.Error.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Error.Name = "Error";
            this.Error.Size = new System.Drawing.Size(41, 17);
            this.Error.TabIndex = 11;
            this.Error.Text = "Error:";
            // 
            // Humidity
            // 
            this.Humidity.AutoSize = true;
            this.Humidity.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Humidity.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Humidity.Location = new System.Drawing.Point(535, 11);
            this.Humidity.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Humidity.Name = "Humidity";
            this.Humidity.Size = new System.Drawing.Size(69, 17);
            this.Humidity.TabIndex = 10;
            this.Humidity.Text = "Humidity:";
            this.Humidity.Click += new System.EventHandler(this.Humidity_Click);
            // 
            // Temperature
            // 
            this.Temperature.AutoSize = true;
            this.Temperature.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Temperature.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Temperature.Location = new System.Drawing.Point(273, 11);
            this.Temperature.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Temperature.Name = "Temperature";
            this.Temperature.Size = new System.Drawing.Size(90, 17);
            this.Temperature.TabIndex = 9;
            this.Temperature.Text = "Temperature:";
            // 
            // SN
            // 
            this.SN.AutoSize = true;
            this.SN.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.SN.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.SN.Location = new System.Drawing.Point(33, 9);
            this.SN.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SN.Name = "SN";
            this.SN.Size = new System.Drawing.Size(32, 17);
            this.SN.TabIndex = 8;
            this.SN.Text = "SN: ";
            // 
            // DisWin
            // 
            this.DisWin.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.DisWin.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DisWin.Location = new System.Drawing.Point(29, 89);
            this.DisWin.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.DisWin.Name = "DisWin";
            this.DisWin.Size = new System.Drawing.Size(649, 388);
            this.DisWin.TabIndex = 13;
            this.DisWin.TabStop = false;
            // 
            // ChannelChecktimer
            // 
            this.ChannelChecktimer.Interval = 20000;
            this.ChannelChecktimer.Tick += new System.EventHandler(this.ChannelChecktimer_Tick);
            // 
            // FunctionBox
            // 
            this.FunctionBox.Controls.Add(this.CMOSCheckBox);
            this.FunctionBox.Controls.Add(this.SingleChannelEnableCheckBox);
            this.FunctionBox.Controls.Add(this.ChannelListComboBox);
            this.FunctionBox.Controls.Add(this.FunctionSelect_YelCu);
            this.FunctionBox.Controls.Add(this.FunctionSelect_NoSelect);
            this.FunctionBox.Controls.Add(this.FunctionSelect_Fe);
            this.FunctionBox.Controls.Add(this.FunctionSelect_Pb);
            this.FunctionBox.Controls.Add(this.FunctionSelect_Zn);
            this.FunctionBox.Controls.Add(this.FunctionSelect_Cu);
            this.FunctionBox.Controls.Add(this.textBox1);
            this.FunctionBox.Cursor = System.Windows.Forms.Cursors.Default;
            this.FunctionBox.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionBox.Location = new System.Drawing.Point(737, 12);
            this.FunctionBox.Name = "FunctionBox";
            this.FunctionBox.Size = new System.Drawing.Size(254, 181);
            this.FunctionBox.TabIndex = 15;
            this.FunctionBox.TabStop = false;
            // 
            // SingleChannelEnableCheckBox
            // 
            this.SingleChannelEnableCheckBox.AutoSize = true;
            this.SingleChannelEnableCheckBox.Location = new System.Drawing.Point(174, 100);
            this.SingleChannelEnableCheckBox.Name = "SingleChannelEnableCheckBox";
            this.SingleChannelEnableCheckBox.Size = new System.Drawing.Size(77, 26);
            this.SingleChannelEnableCheckBox.TabIndex = 23;
            this.SingleChannelEnableCheckBox.Text = "单通道";
            this.SingleChannelEnableCheckBox.UseVisualStyleBackColor = true;
            this.SingleChannelEnableCheckBox.CheckedChanged += new System.EventHandler(this.SingleChannelEnableCheckBox_CheckedChanged);
            // 
            // ChannelListComboBox
            // 
            this.ChannelListComboBox.Enabled = false;
            this.ChannelListComboBox.FormattingEnabled = true;
            this.ChannelListComboBox.Location = new System.Drawing.Point(174, 136);
            this.ChannelListComboBox.Name = "ChannelListComboBox";
            this.ChannelListComboBox.Size = new System.Drawing.Size(47, 30);
            this.ChannelListComboBox.TabIndex = 22;
            this.ChannelListComboBox.SelectedIndexChanged += new System.EventHandler(this.ChannelListComboBox_SelectedIndexChanged);
            // 
            // FunctionSelect_YelCu
            // 
            this.FunctionSelect_YelCu.AutoSize = true;
            this.FunctionSelect_YelCu.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FunctionSelect_YelCu.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionSelect_YelCu.Location = new System.Drawing.Point(19, 143);
            this.FunctionSelect_YelCu.Name = "FunctionSelect_YelCu";
            this.FunctionSelect_YelCu.Size = new System.Drawing.Size(56, 23);
            this.FunctionSelect_YelCu.TabIndex = 21;
            this.FunctionSelect_YelCu.Text = "黄铜";
            this.FunctionSelect_YelCu.UseVisualStyleBackColor = true;
            // 
            // FunctionSelect_NoSelect
            // 
            this.FunctionSelect_NoSelect.AutoSize = true;
            this.FunctionSelect_NoSelect.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FunctionSelect_NoSelect.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionSelect_NoSelect.Location = new System.Drawing.Point(81, 143);
            this.FunctionSelect_NoSelect.Name = "FunctionSelect_NoSelect";
            this.FunctionSelect_NoSelect.Size = new System.Drawing.Size(70, 23);
            this.FunctionSelect_NoSelect.TabIndex = 20;
            this.FunctionSelect_NoSelect.Text = "不分选";
            this.FunctionSelect_NoSelect.UseVisualStyleBackColor = true;
            // 
            // FunctionSelect_Fe
            // 
            this.FunctionSelect_Fe.AutoSize = true;
            this.FunctionSelect_Fe.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FunctionSelect_Fe.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionSelect_Fe.Location = new System.Drawing.Point(81, 103);
            this.FunctionSelect_Fe.Name = "FunctionSelect_Fe";
            this.FunctionSelect_Fe.Size = new System.Drawing.Size(70, 23);
            this.FunctionSelect_Fe.TabIndex = 19;
            this.FunctionSelect_Fe.Text = "不锈钢";
            this.FunctionSelect_Fe.UseVisualStyleBackColor = true;
            // 
            // FunctionSelect_Pb
            // 
            this.FunctionSelect_Pb.AutoSize = true;
            this.FunctionSelect_Pb.Checked = true;
            this.FunctionSelect_Pb.CheckState = System.Windows.Forms.CheckState.Checked;
            this.FunctionSelect_Pb.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FunctionSelect_Pb.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionSelect_Pb.Location = new System.Drawing.Point(19, 103);
            this.FunctionSelect_Pb.Name = "FunctionSelect_Pb";
            this.FunctionSelect_Pb.Size = new System.Drawing.Size(42, 23);
            this.FunctionSelect_Pb.TabIndex = 18;
            this.FunctionSelect_Pb.Text = "铅";
            this.FunctionSelect_Pb.UseVisualStyleBackColor = true;
            // 
            // FunctionSelect_Zn
            // 
            this.FunctionSelect_Zn.AutoSize = true;
            this.FunctionSelect_Zn.Checked = true;
            this.FunctionSelect_Zn.CheckState = System.Windows.Forms.CheckState.Checked;
            this.FunctionSelect_Zn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FunctionSelect_Zn.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionSelect_Zn.Location = new System.Drawing.Point(81, 63);
            this.FunctionSelect_Zn.Name = "FunctionSelect_Zn";
            this.FunctionSelect_Zn.Size = new System.Drawing.Size(42, 23);
            this.FunctionSelect_Zn.TabIndex = 17;
            this.FunctionSelect_Zn.Text = "锌";
            this.FunctionSelect_Zn.UseVisualStyleBackColor = true;
            // 
            // FunctionSelect_Cu
            // 
            this.FunctionSelect_Cu.AutoSize = true;
            this.FunctionSelect_Cu.Checked = true;
            this.FunctionSelect_Cu.CheckState = System.Windows.Forms.CheckState.Checked;
            this.FunctionSelect_Cu.Cursor = System.Windows.Forms.Cursors.Hand;
            this.FunctionSelect_Cu.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FunctionSelect_Cu.Location = new System.Drawing.Point(19, 63);
            this.FunctionSelect_Cu.Name = "FunctionSelect_Cu";
            this.FunctionSelect_Cu.Size = new System.Drawing.Size(42, 23);
            this.FunctionSelect_Cu.TabIndex = 16;
            this.FunctionSelect_Cu.Text = "铜";
            this.FunctionSelect_Cu.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox1.BackColor = System.Drawing.Color.BurlyWood;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Enabled = false;
            this.textBox1.Font = new System.Drawing.Font("微软雅黑", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox1.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.textBox1.Location = new System.Drawing.Point(34, 24);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(188, 26);
            this.textBox1.TabIndex = 15;
            this.textBox1.Text = "分选功能选择";
            this.textBox1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Version
            // 
            this.Version.BackColor = System.Drawing.Color.BurlyWood;
            this.Version.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.Version.Enabled = false;
            this.Version.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Version.ForeColor = System.Drawing.SystemColors.Control;
            this.Version.Location = new System.Drawing.Point(911, 489);
            this.Version.Name = "Version";
            this.Version.Size = new System.Drawing.Size(124, 14);
            this.Version.TabIndex = 16;
            this.Version.Text = "Version: 1.5.6";
            // 
            // paraSetButton
            // 
            this.paraSetButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.paraSetButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.paraSetButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.paraSetButton.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.paraSetButton.Location = new System.Drawing.Point(879, 217);
            this.paraSetButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.paraSetButton.Name = "paraSetButton";
            this.paraSetButton.Size = new System.Drawing.Size(124, 61);
            this.paraSetButton.TabIndex = 17;
            this.paraSetButton.Tag = "0";
            this.paraSetButton.Text = "参数设置";
            this.paraSetButton.UseVisualStyleBackColor = false;
            this.paraSetButton.Click += new System.EventHandler(this.paraSetButton_Click);
            // 
            // refreshThreshButton
            // 
            this.refreshThreshButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.refreshThreshButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.refreshThreshButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.refreshThreshButton.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.refreshThreshButton.Location = new System.Drawing.Point(879, 317);
            this.refreshThreshButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.refreshThreshButton.Name = "refreshThreshButton";
            this.refreshThreshButton.Size = new System.Drawing.Size(124, 61);
            this.refreshThreshButton.TabIndex = 18;
            this.refreshThreshButton.Tag = "0";
            this.refreshThreshButton.Text = "刷新阈值";
            this.refreshThreshButton.UseVisualStyleBackColor = false;
            this.refreshThreshButton.Click += new System.EventHandler(this.refreshThreshButton_Click);
            // 
            // PowerPanelEnableButton
            // 
            this.PowerPanelEnableButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.PowerPanelEnableButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.PowerPanelEnableButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.PowerPanelEnableButton.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.PowerPanelEnableButton.Location = new System.Drawing.Point(879, 417);
            this.PowerPanelEnableButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.PowerPanelEnableButton.Name = "PowerPanelEnableButton";
            this.PowerPanelEnableButton.Size = new System.Drawing.Size(124, 61);
            this.PowerPanelEnableButton.TabIndex = 19;
            this.PowerPanelEnableButton.Tag = "0";
            this.PowerPanelEnableButton.Text = "功放面板";
            this.PowerPanelEnableButton.UseVisualStyleBackColor = false;
            this.PowerPanelEnableButton.Click += new System.EventHandler(this.PowerPanelEnableButton_Click);
            // 
            // CMOSCheckBox
            // 
            this.CMOSCheckBox.AutoSize = true;
            this.CMOSCheckBox.Checked = true;
            this.CMOSCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CMOSCheckBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CMOSCheckBox.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.CMOSCheckBox.Location = new System.Drawing.Point(174, 63);
            this.CMOSCheckBox.Name = "CMOSCheckBox";
            this.CMOSCheckBox.Size = new System.Drawing.Size(70, 23);
            this.CMOSCheckBox.TabIndex = 24;
            this.CMOSCheckBox.Text = "CMOS";
            this.CMOSCheckBox.UseVisualStyleBackColor = true;
            // 
            // TestForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.BurlyWood;
            this.ClientSize = new System.Drawing.Size(1027, 511);
            this.Controls.Add(this.PowerPanelEnableButton);
            this.Controls.Add(this.refreshThreshButton);
            this.Controls.Add(this.paraSetButton);
            this.Controls.Add(this.Version);
            this.Controls.Add(this.FunctionBox);
            this.Controls.Add(this.DisWin);
            this.Controls.Add(this.LostLine);
            this.Controls.Add(this.Error);
            this.Controls.Add(this.Humidity);
            this.Controls.Add(this.Temperature);
            this.Controls.Add(this.SN);
            this.Controls.Add(this.StopButton);
            this.Controls.Add(this.StartButton);
            this.Controls.Add(this.FindDeviceButton);
            this.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "TestForm";
            this.Text = "工业物料喷吹分拣系统";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TestForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.TestForm_FormClosed);
            this.Load += new System.EventHandler(this.TestForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.DisWin)).EndInit();
            this.FunctionBox.ResumeLayout(false);
            this.FunctionBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button FindDeviceButton;
        private System.Windows.Forms.Button StartButton;
        private System.Windows.Forms.Button StopButton;
        private System.Windows.Forms.Label LostLine;
        private System.Windows.Forms.Label Error;
        private System.Windows.Forms.Label Humidity;
        private System.Windows.Forms.Label Temperature;
        private System.Windows.Forms.Label SN;
        private System.Windows.Forms.PictureBox DisWin;
        private System.Windows.Forms.Timer ChannelChecktimer;
        private System.Windows.Forms.GroupBox FunctionBox;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.CheckBox FunctionSelect_Fe;
        private System.Windows.Forms.CheckBox FunctionSelect_Pb;
        private System.Windows.Forms.CheckBox FunctionSelect_Zn;
        private System.Windows.Forms.CheckBox FunctionSelect_Cu;
        private System.Windows.Forms.CheckBox FunctionSelect_NoSelect;
        private System.Windows.Forms.TextBox Version;
        private System.Windows.Forms.CheckBox FunctionSelect_YelCu;
        private System.Windows.Forms.Button paraSetButton;
        private System.Windows.Forms.Button refreshThreshButton;
        private System.Windows.Forms.ComboBox ChannelListComboBox;
        private System.Windows.Forms.Button PowerPanelEnableButton;
        private System.Windows.Forms.CheckBox SingleChannelEnableCheckBox;
        private System.Windows.Forms.CheckBox CMOSCheckBox;
    }
}