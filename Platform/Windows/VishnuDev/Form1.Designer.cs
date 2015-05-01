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
            this.GLES2View = new VishnuDev.VishnuGLES2();
            this.SuspendLayout();
            // 
            // btnBackground
            // 
            this.btnBackground.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBackground.Appearance = System.Windows.Forms.Appearance.Button;
            this.btnBackground.Enabled = false;
            this.btnBackground.Font = new System.Drawing.Font("メイリオ", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnBackground.Location = new System.Drawing.Point(527, 344);
            this.btnBackground.Name = "btnBackground";
            this.btnBackground.Size = new System.Drawing.Size(85, 26);
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
            this.btnPlay.Location = new System.Drawing.Point(527, 312);
            this.btnPlay.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(26, 26);
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
            this.btnPause.Location = new System.Drawing.Point(557, 312);
            this.btnPause.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(26, 26);
            this.btnPause.TabIndex = 2;
            this.btnPause.UseVisualStyleBackColor = true;
            this.btnPause.CheckedChanged += new System.EventHandler(this.btnPause_CheckedChanged);
            // 
            // btnStep
            // 
            this.btnStep.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnStep.Enabled = false;
            this.btnStep.Image = ((System.Drawing.Image)(resources.GetObject("btnStep.Image")));
            this.btnStep.Location = new System.Drawing.Point(586, 312);
            this.btnStep.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.btnStep.Name = "btnStep";
            this.btnStep.Size = new System.Drawing.Size(26, 26);
            this.btnStep.TabIndex = 3;
            this.btnStep.UseVisualStyleBackColor = true;
            this.btnStep.Click += new System.EventHandler(this.btnStep_Click);
            // 
            // logText
            // 
            this.logText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.logText.Location = new System.Drawing.Point(12, 312);
            this.logText.Multiline = true;
            this.logText.Name = "logText";
            this.logText.ReadOnly = true;
            this.logText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logText.Size = new System.Drawing.Size(509, 96);
            this.logText.TabIndex = 6;
            this.logText.TextChanged += new System.EventHandler(this.logText_TextChanged);
            // 
            // btnDirectory
            // 
            this.btnDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnDirectory.Image = ((System.Drawing.Image)(resources.GetObject("btnDirectory.Image")));
            this.btnDirectory.Location = new System.Drawing.Point(586, 381);
            this.btnDirectory.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.btnDirectory.Name = "btnDirectory";
            this.btnDirectory.Size = new System.Drawing.Size(26, 26);
            this.btnDirectory.TabIndex = 7;
            this.btnDirectory.UseVisualStyleBackColor = true;
            this.btnDirectory.Click += new System.EventHandler(this.btnDirectory_Click);
            // 
            // lblFPS
            // 
            this.lblFPS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lblFPS.AutoSize = true;
            this.lblFPS.Location = new System.Drawing.Point(528, 394);
            this.lblFPS.Name = "lblFPS";
            this.lblFPS.Size = new System.Drawing.Size(29, 12);
            this.lblFPS.TabIndex = 8;
            this.lblFPS.Text = "(fps)";
            this.lblFPS.Click += new System.EventHandler(this.lblFPS_Click);
            // 
            // GLES2View
            // 
            this.GLES2View.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.GLES2View.BackColor = System.Drawing.SystemColors.Desktop;
            this.GLES2View.Location = new System.Drawing.Point(12, 12);
            this.GLES2View.Name = "GLES2View";
            this.GLES2View.Size = new System.Drawing.Size(600, 293);
            this.GLES2View.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(624, 420);
            this.Controls.Add(this.lblFPS);
            this.Controls.Add(this.btnDirectory);
            this.Controls.Add(this.btnBackground);
            this.Controls.Add(this.btnStep);
            this.Controls.Add(this.btnPlay);
            this.Controls.Add(this.btnPause);
            this.Controls.Add(this.logText);
            this.Controls.Add(this.GLES2View);
            this.Name = "Form1";
            this.Text = "VishnuDevelop";
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


    }
}

