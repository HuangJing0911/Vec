namespace XEthernetDemo
{
    partial class StartMenu
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(StartMenu));
            this.DebugButton = new System.Windows.Forms.Button();
            this.SettingButton = new System.Windows.Forms.Button();
            this.TestButton = new System.Windows.Forms.Button();
            this.LocalAdapter = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.Speed = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.MouthNum = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.BeltLength = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.ArrayLength = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.SOD = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.SDD = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.Power_Amplifier = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.ChangeSetiingButton = new System.Windows.Forms.Button();
            this.ReturnButton = new System.Windows.Forms.Button();
            this.PLC_IP = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.SystemName = new System.Windows.Forms.Label();
            this.gap = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.Version = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // DebugButton
            // 
            this.DebugButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.DebugButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.DebugButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.DebugButton.Location = new System.Drawing.Point(72, 208);
            this.DebugButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.DebugButton.Name = "DebugButton";
            this.DebugButton.Size = new System.Drawing.Size(141, 49);
            this.DebugButton.TabIndex = 2;
            this.DebugButton.Tag = "0";
            this.DebugButton.Text = "调试版本";
            this.DebugButton.UseVisualStyleBackColor = false;
            this.DebugButton.Click += new System.EventHandler(this.DebugButton_Click);
            // 
            // SettingButton
            // 
            this.SettingButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.SettingButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.SettingButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.SettingButton.Location = new System.Drawing.Point(250, 208);
            this.SettingButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.SettingButton.Name = "SettingButton";
            this.SettingButton.Size = new System.Drawing.Size(191, 49);
            this.SettingButton.TabIndex = 3;
            this.SettingButton.Tag = "0";
            this.SettingButton.Text = "参数查看与设置";
            this.SettingButton.UseVisualStyleBackColor = false;
            this.SettingButton.Click += new System.EventHandler(this.SettingButton_Click);
            // 
            // TestButton
            // 
            this.TestButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.TestButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.TestButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.TestButton.Location = new System.Drawing.Point(493, 208);
            this.TestButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.TestButton.Name = "TestButton";
            this.TestButton.Size = new System.Drawing.Size(141, 49);
            this.TestButton.TabIndex = 4;
            this.TestButton.Tag = "0";
            this.TestButton.Text = "测试版本";
            this.TestButton.UseVisualStyleBackColor = false;
            this.TestButton.Click += new System.EventHandler(this.TestButton_Click);
            // 
            // LocalAdapter
            // 
            this.LocalAdapter.BackColor = System.Drawing.Color.BurlyWood;
            this.LocalAdapter.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.LocalAdapter.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.LocalAdapter.Location = new System.Drawing.Point(162, 62);
            this.LocalAdapter.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.LocalAdapter.Name = "LocalAdapter";
            this.LocalAdapter.Size = new System.Drawing.Size(148, 21);
            this.LocalAdapter.TabIndex = 6;
            this.LocalAdapter.Tag = "1";
            this.LocalAdapter.Text = "192.168.1.10";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.BurlyWood;
            this.label1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label1.Location = new System.Drawing.Point(50, 66);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 12);
            this.label1.TabIndex = 5;
            this.label1.Tag = "1";
            this.label1.Text = "线阵IP地址：";
            // 
            // Speed
            // 
            this.Speed.BackColor = System.Drawing.Color.BurlyWood;
            this.Speed.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Speed.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Speed.Location = new System.Drawing.Point(162, 102);
            this.Speed.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Speed.MaxLength = 3;
            this.Speed.Name = "Speed";
            this.Speed.Size = new System.Drawing.Size(148, 21);
            this.Speed.TabIndex = 8;
            this.Speed.Tag = "1";
            this.Speed.Text = "3.0";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.BurlyWood;
            this.label2.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label2.Location = new System.Drawing.Point(50, 107);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(83, 12);
            this.label2.TabIndex = 7;
            this.label2.Tag = "1";
            this.label2.Text = "速度（米）：";
            // 
            // MouthNum
            // 
            this.MouthNum.BackColor = System.Drawing.Color.BurlyWood;
            this.MouthNum.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.MouthNum.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.MouthNum.Location = new System.Drawing.Point(162, 139);
            this.MouthNum.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.MouthNum.Name = "MouthNum";
            this.MouthNum.Size = new System.Drawing.Size(148, 21);
            this.MouthNum.TabIndex = 10;
            this.MouthNum.Tag = "1";
            this.MouthNum.Text = "198";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.BurlyWood;
            this.label3.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label3.Location = new System.Drawing.Point(50, 144);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(109, 12);
            this.label3.TabIndex = 9;
            this.label3.Tag = "1";
            this.label3.Text = "喷嘴数量（个）：";
            // 
            // BeltLength
            // 
            this.BeltLength.BackColor = System.Drawing.Color.BurlyWood;
            this.BeltLength.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.BeltLength.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.BeltLength.Location = new System.Drawing.Point(162, 179);
            this.BeltLength.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.BeltLength.Name = "BeltLength";
            this.BeltLength.Size = new System.Drawing.Size(148, 21);
            this.BeltLength.TabIndex = 12;
            this.BeltLength.Tag = "1";
            this.BeltLength.Text = "1000";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.BurlyWood;
            this.label4.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label4.Location = new System.Drawing.Point(50, 184);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(110, 12);
            this.label4.TabIndex = 11;
            this.label4.Tag = "1";
            this.label4.Text = "皮带长度（mm）：";
            // 
            // ArrayLength
            // 
            this.ArrayLength.BackColor = System.Drawing.Color.BurlyWood;
            this.ArrayLength.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ArrayLength.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ArrayLength.Location = new System.Drawing.Point(162, 223);
            this.ArrayLength.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.ArrayLength.Name = "ArrayLength";
            this.ArrayLength.Size = new System.Drawing.Size(148, 21);
            this.ArrayLength.TabIndex = 14;
            this.ArrayLength.Tag = "1";
            this.ArrayLength.Text = "1120";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.Color.BurlyWood;
            this.label5.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label5.Location = new System.Drawing.Point(50, 228);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 12);
            this.label5.TabIndex = 13;
            this.label5.Tag = "1";
            this.label5.Text = "线阵长度（mm）：";
            // 
            // SOD
            // 
            this.SOD.BackColor = System.Drawing.Color.BurlyWood;
            this.SOD.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.SOD.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.SOD.Location = new System.Drawing.Point(535, 139);
            this.SOD.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SOD.Name = "SOD";
            this.SOD.Size = new System.Drawing.Size(148, 21);
            this.SOD.TabIndex = 16;
            this.SOD.Tag = "1";
            this.SOD.Text = "815";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.BackColor = System.Drawing.Color.BurlyWood;
            this.label6.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label6.Location = new System.Drawing.Point(404, 144);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(136, 12);
            this.label6.TabIndex = 15;
            this.label6.Tag = "1";
            this.label6.Text = "皮带到源距离（mm）：";
            // 
            // SDD
            // 
            this.SDD.BackColor = System.Drawing.Color.BurlyWood;
            this.SDD.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.SDD.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.SDD.Location = new System.Drawing.Point(535, 181);
            this.SDD.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SDD.Name = "SDD";
            this.SDD.Size = new System.Drawing.Size(148, 21);
            this.SDD.TabIndex = 18;
            this.SDD.Tag = "1";
            this.SDD.Text = "914";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.BackColor = System.Drawing.Color.BurlyWood;
            this.label7.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label7.Location = new System.Drawing.Point(404, 185);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(136, 12);
            this.label7.TabIndex = 17;
            this.label7.Tag = "1";
            this.label7.Text = "线阵到源距离（mm）：";
            // 
            // Power_Amplifier
            // 
            this.Power_Amplifier.BackColor = System.Drawing.Color.BurlyWood;
            this.Power_Amplifier.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Power_Amplifier.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Power_Amplifier.Location = new System.Drawing.Point(535, 62);
            this.Power_Amplifier.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Power_Amplifier.Name = "Power_Amplifier";
            this.Power_Amplifier.Size = new System.Drawing.Size(148, 21);
            this.Power_Amplifier.TabIndex = 20;
            this.Power_Amplifier.Tag = "1";
            this.Power_Amplifier.Text = "172.28.110.50";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.Color.BurlyWood;
            this.label8.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label8.Location = new System.Drawing.Point(404, 66);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(84, 12);
            this.label8.TabIndex = 19;
            this.label8.Tag = "1";
            this.label8.Text = "功放IP地址：";
            // 
            // ChangeSetiingButton
            // 
            this.ChangeSetiingButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.ChangeSetiingButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ChangeSetiingButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ChangeSetiingButton.Location = new System.Drawing.Point(178, 304);
            this.ChangeSetiingButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ChangeSetiingButton.Name = "ChangeSetiingButton";
            this.ChangeSetiingButton.Size = new System.Drawing.Size(141, 49);
            this.ChangeSetiingButton.TabIndex = 21;
            this.ChangeSetiingButton.Tag = "2";
            this.ChangeSetiingButton.Text = "更改参数";
            this.ChangeSetiingButton.UseVisualStyleBackColor = false;
            this.ChangeSetiingButton.Click += new System.EventHandler(this.ChangeSetiingButton_Click);
            // 
            // ReturnButton
            // 
            this.ReturnButton.BackColor = System.Drawing.Color.AntiqueWhite;
            this.ReturnButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ReturnButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ReturnButton.Location = new System.Drawing.Point(406, 304);
            this.ReturnButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ReturnButton.Name = "ReturnButton";
            this.ReturnButton.Size = new System.Drawing.Size(141, 49);
            this.ReturnButton.TabIndex = 22;
            this.ReturnButton.Tag = "2";
            this.ReturnButton.Text = "返回";
            this.ReturnButton.UseVisualStyleBackColor = false;
            this.ReturnButton.Click += new System.EventHandler(this.ReturnButton_Click);
            // 
            // PLC_IP
            // 
            this.PLC_IP.BackColor = System.Drawing.Color.BurlyWood;
            this.PLC_IP.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.PLC_IP.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.PLC_IP.Location = new System.Drawing.Point(535, 98);
            this.PLC_IP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.PLC_IP.Name = "PLC_IP";
            this.PLC_IP.Size = new System.Drawing.Size(148, 21);
            this.PLC_IP.TabIndex = 24;
            this.PLC_IP.Tag = "1";
            this.PLC_IP.Text = "192.168.250.1";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.BackColor = System.Drawing.Color.BurlyWood;
            this.label9.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label9.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label9.Location = new System.Drawing.Point(404, 102);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(86, 12);
            this.label9.TabIndex = 23;
            this.label9.Tag = "1";
            this.label9.Text = "PLC IP地址：";
            // 
            // SystemName
            // 
            this.SystemName.AutoSize = true;
            this.SystemName.Enabled = false;
            this.SystemName.Font = new System.Drawing.Font("微软雅黑", 30F, System.Drawing.FontStyle.Bold);
            this.SystemName.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.SystemName.Location = new System.Drawing.Point(155, 78);
            this.SystemName.Name = "SystemName";
            this.SystemName.Size = new System.Drawing.Size(422, 52);
            this.SystemName.TabIndex = 25;
            this.SystemName.Tag = "0";
            this.SystemName.Text = "工业物料喷吹分拣系统";
            // 
            // gap
            // 
            this.gap.BackColor = System.Drawing.Color.BurlyWood;
            this.gap.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.gap.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.gap.Location = new System.Drawing.Point(535, 228);
            this.gap.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.gap.Name = "gap";
            this.gap.Size = new System.Drawing.Size(148, 21);
            this.gap.TabIndex = 27;
            this.gap.Tag = "1";
            this.gap.Text = "8";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.BackColor = System.Drawing.Color.BurlyWood;
            this.label10.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label10.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label10.Location = new System.Drawing.Point(404, 232);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(70, 12);
            this.label10.TabIndex = 26;
            this.label10.Tag = "1";
            this.label10.Text = "功放阈值：";
            // 
            // Version
            // 
            this.Version.BackColor = System.Drawing.Color.BurlyWood;
            this.Version.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.Version.Enabled = false;
            this.Version.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Version.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Version.Location = new System.Drawing.Point(623, 338);
            this.Version.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Version.Name = "Version";
            this.Version.ReadOnly = true;
            this.Version.Size = new System.Drawing.Size(110, 14);
            this.Version.TabIndex = 28;
            this.Version.Tag = "0";
            this.Version.Text = "Version: 1.6.2";
            // 
            // StartMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.BurlyWood;
            this.ClientSize = new System.Drawing.Size(736, 362);
            this.Controls.Add(this.Version);
            this.Controls.Add(this.gap);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.PLC_IP);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.ReturnButton);
            this.Controls.Add(this.ChangeSetiingButton);
            this.Controls.Add(this.Power_Amplifier);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.SDD);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.SOD);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.ArrayLength);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.BeltLength);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.MouthNum);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.Speed);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.LocalAdapter);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.TestButton);
            this.Controls.Add(this.SettingButton);
            this.Controls.Add(this.DebugButton);
            this.Controls.Add(this.SystemName);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MaximizeBox = false;
            this.Name = "StartMenu";
            this.Text = "工业物料喷吹分拣系统";
            this.Load += new System.EventHandler(this.StartMenu_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button DebugButton;
        private System.Windows.Forms.Button SettingButton;
        private System.Windows.Forms.Button TestButton;
        private System.Windows.Forms.TextBox LocalAdapter;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox Speed;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox MouthNum;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox BeltLength;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox ArrayLength;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox SOD;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox SDD;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox Power_Amplifier;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button ChangeSetiingButton;
        private System.Windows.Forms.Button ReturnButton;
        private System.Windows.Forms.TextBox PLC_IP;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label SystemName;
        private System.Windows.Forms.TextBox gap;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox Version;
    }
}