namespace VishnuDev
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnBackground = new System.Windows.Forms.CheckBox();
            this.btnPlay = new System.Windows.Forms.CheckBox();
            this.btnPause = new System.Windows.Forms.CheckBox();
            this.btnStep = new System.Windows.Forms.Button();
            this.logText = new System.Windows.Forms.TextBox();
            this.btnDirectory = new System.Windows.Forms.Button();
            this.lblFPS = new System.Windows.Forms.Label();
            this.comboDisplay = new System.Windows.Forms.ComboBox();
            this.chkDisplay = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.chkVREnv = new System.Windows.Forms.CheckBox();
            this.GLES2View = new VishnuDev.VishnuGLES2();
            this.SuspendLayout();
            // 
            // btnBackground
            // 
            this.btnBackground.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBackground.Appearance = System.Windows.Forms.Appearance.Button;
            this.btnBackground.Enabled = false;
            this.btnBackground.Font = new System.Drawing.Font("メイリオ", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnBackground.Location = new System.Drawing.Point(875, 538);
            this.btnBackground.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.btnBackground.Name = "btnBackground";
            this.btnBackground.Size = new System.Drawing.Size(142, 39);
            this.btnBackground.TabIndex = 5;
            this.btnBackground.Text = "Foreground";
            this.btnBackground.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.btnBackground.UseVisualStyleBackColor = true;
            this.btnBackground.CheckedChanged += new System.EventHandler(this.btnBackground_CheckedChanged);
            // 
            // btnPlay
            // 
            this.btnPlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPlay.Appearance = System.Windows.Forms.Appearance.Button;
            this.btnPlay.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.btnPlay.Image = ((System.Drawing.Image)(resources.GetObject("btnPlay.Image")));
            this.btnPlay.Location = new System.Drawing.Point(875, 491);
            this.btnPlay.Margin = new System.Windows.Forms.Padding(0, 4, 0, 4);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(43, 39);
            this.btnPlay.TabIndex = 1;
            this.btnPlay.UseVisualStyleBackColor = true;
            this.btnPlay.CheckedChanged += new System.EventHandler(this.btnPlay_CheckedChanged);
            // 
            // btnPause
            // 
            this.btnPause.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPause.Appearance = System.Windows.Forms.Appearance.Button;
            this.btnPause.Enabled = false;
            this.btnPause.Image = ((System.Drawing.Image)(resources.GetObject("btnPause.Image")));
            this.btnPause.Location = new System.Drawing.Point(925, 491);
            this.btnPause.Margin = new System.Windows.Forms.Padding(0, 4, 0, 4);
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(43, 39);
            this.btnPause.TabIndex = 2;
            this.btnPause.UseVisualStyleBackColor = true;
            this.btnPause.CheckedChanged += new System.EventHandler(this.btnPause_CheckedChanged);
            // 
            // btnStep
            // 
            this.btnStep.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnStep.Enabled = false;
            this.btnStep.Image = ((System.Drawing.Image)(resources.GetObject("btnStep.Image")));
            this.btnStep.Location = new System.Drawing.Point(974, 491);
            this.btnStep.Margin = new System.Windows.Forms.Padding(0, 4, 0, 4);
            this.btnStep.Name = "btnStep";
            this.btnStep.Size = new System.Drawing.Size(43, 39);
            this.btnStep.TabIndex = 3;
            this.btnStep.UseVisualStyleBackColor = true;
            this.btnStep.Click += new System.EventHandler(this.btnStep_Click);
            // 
            // logText
            // 
            this.logText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.logText.Location = new System.Drawing.Point(20, 491);
            this.logText.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.logText.Multiline = true;
            this.logText.Name = "logText";
            this.logText.ReadOnly = true;
            this.logText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logText.Size = new System.Drawing.Size(848, 142);
            this.logText.TabIndex = 6;
            this.logText.TextChanged += new System.EventHandler(this.logText_TextChanged);
            // 
            // btnDirectory
            // 
            this.btnDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnDirectory.Image = ((System.Drawing.Image)(resources.GetObject("btnDirectory.Image")));
            this.btnDirectory.Location = new System.Drawing.Point(974, 594);
            this.btnDirectory.Margin = new System.Windows.Forms.Padding(0, 4, 0, 4);
            this.btnDirectory.Name = "btnDirectory";
            this.btnDirectory.Size = new System.Drawing.Size(43, 39);
            this.btnDirectory.TabIndex = 7;
            this.btnDirectory.UseVisualStyleBackColor = true;
            this.btnDirectory.Click += new System.EventHandler(this.btnDirectory_Click);
            // 
            // lblFPS
            // 
            this.lblFPS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lblFPS.AutoSize = true;
            this.lblFPS.Location = new System.Drawing.Point(877, 613);
            this.lblFPS.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.lblFPS.Name = "lblFPS";
            this.lblFPS.Size = new System.Drawing.Size(40, 18);
            this.lblFPS.TabIndex = 8;
            this.lblFPS.Text = "(fps)";
            this.lblFPS.Click += new System.EventHandler(this.lblFPS_Click);
            // 
            // comboDisplay
            // 
            this.comboDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.comboDisplay.Enabled = false;
            this.comboDisplay.FormattingEnabled = true;
            this.comboDisplay.Location = new System.Drawing.Point(265, 646);
            this.comboDisplay.Name = "comboDisplay";
            this.comboDisplay.Size = new System.Drawing.Size(314, 26);
            this.comboDisplay.TabIndex = 10;
            // 
            // chkDisplay
            // 
            this.chkDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chkDisplay.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkDisplay.Enabled = false;
            this.chkDisplay.Location = new System.Drawing.Point(20, 644);
            this.chkDisplay.Name = "chkDisplay";
            this.chkDisplay.Size = new System.Drawing.Size(142, 39);
            this.chkDisplay.TabIndex = 11;
            this.chkDisplay.Text = "to VR";
            this.chkDisplay.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkDisplay.UseVisualStyleBackColor = true;
            this.chkDisplay.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(168, 654);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 18);
            this.label1.TabIndex = 12;
            this.label1.Text = "VR display:";
            // 
            // chkVREnv
            // 
            this.chkVREnv.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chkVREnv.AutoSize = true;
            this.chkVREnv.Location = new System.Drawing.Point(594, 650);
            this.chkVREnv.Name = "chkVREnv";
            this.chkVREnv.Size = new System.Drawing.Size(154, 22);
            this.chkVREnv.TabIndex = 13;
            this.chkVREnv.Text = "VR Environment";
            this.chkVREnv.UseVisualStyleBackColor = true;
            this.chkVREnv.CheckedChanged += new System.EventHandler(this.chkVREnv_CheckedChanged);
            // 
            // GLES2View
            // 
            this.GLES2View.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.GLES2View.BackColor = System.Drawing.SystemColors.Desktop;
            this.GLES2View.Location = new System.Drawing.Point(20, 18);
            this.GLES2View.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.GLES2View.Name = "GLES2View";
            this.GLES2View.Size = new System.Drawing.Size(1000, 465);
            this.GLES2View.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1040, 689);
            this.Controls.Add(this.chkVREnv);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.chkDisplay);
            this.Controls.Add(this.comboDisplay);
            this.Controls.Add(this.lblFPS);
            this.Controls.Add(this.btnDirectory);
            this.Controls.Add(this.btnBackground);
            this.Controls.Add(this.btnStep);
            this.Controls.Add(this.btnPlay);
            this.Controls.Add(this.btnPause);
            this.Controls.Add(this.logText);
            this.Controls.Add(this.GLES2View);
            this.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.Name = "Form1";
            this.Text = "VishnuDevelop";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private VishnuGLES2 GLES2View;
        private System.Windows.Forms.CheckBox btnPlay;
        private System.Windows.Forms.CheckBox btnPause;
        private System.Windows.Forms.Button btnStep;
        private System.Windows.Forms.CheckBox btnBackground;
        private System.Windows.Forms.TextBox logText;
        private System.Windows.Forms.Button btnDirectory;
        private System.Windows.Forms.Label lblFPS;
        private System.Windows.Forms.ComboBox comboDisplay;
        private System.Windows.Forms.CheckBox chkDisplay;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox chkVREnv;


    }
}

