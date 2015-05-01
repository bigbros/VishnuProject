using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace VishnuDev
{
    public partial class Form1 : Form
    {
        private bool running = false;
        private bool background = false;
        private Timer watchTimer = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void LogOutput(string msg)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append(logText.Text);
            builder.Append(msg);
            builder.Append(System.Environment.NewLine);
            logText.Text = builder.ToString();
            logText.SelectionStart = logText.Text.Length;
            logText.Focus();
            logText.ScrollToCaret();
        }


        private bool selectPath()
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "ターゲットフォルダ選択";
            fbd.RootFolder = Environment.SpecialFolder.Desktop;
            fbd.SelectedPath = VishnuSystem.Instance.Path;
            fbd.ShowNewFolderButton = false;

            if (fbd.ShowDialog(this) == DialogResult.OK)
            {
                string path = fbd.SelectedPath;
                VishnuSystem.Instance.setPath(path);
                return true;
            }
            return false;
        }


        protected void OnWatch(Object sender, EventArgs args)
        {
            double fps = GLES2View.nowFps;
            lblFPS.Text = string.Format("{0}[fps]", fps.ToString("0.00"));
        }


        private bool gameRun()
        {
            if (running) return true;
            if (VishnuSystem.Instance.Path == null)
            {
                if(!selectPath()) return false;
            }
            logText.Text = "";
            VishnuSystem.Instance.setLogMethod(LogOutput);
            GLES2View.RestoreGLES2();
            VishnuSystem.Instance.Start();
            GLES2View.Start();
            running = true;

            if (watchTimer == null)
            {
                watchTimer = new Timer();
                watchTimer.Tick += OnWatch;
            }
            watchTimer.Interval = 500;  // 0.5秒
            watchTimer.Start();
            return true;
        }

        private void gameBreak()
        {
            if (!running) return;
            if (background)
            {
                GLES2View.CreateSurface();
                background = false;
            }
            GLES2View.Stop();
            watchTimer.Stop();
            lblFPS.Text = "";
            VishnuSystem.Instance.Stop();
            GLES2View.DestroyGLES2();
            running = false;
        }

        private void gameToBackground()
        {
            if (background) return;
            GLES2View.Stop();
            VishnuSystem.Instance.OnPause();
            GLES2View.DestroySurface();
            background = true;
        }

        private void gameToForeground()
        {
            if (!background) return;
            GLES2View.CreateSurface();
            VishnuSystem.Instance.OnResume();
            GLES2View.Start();
            background = false;
        }

        private void btnPlay_CheckedChanged(object sender, EventArgs e)
        {
            btnStep.Enabled = false;
            btnPause.Checked = false;
            btnBackground.Checked = false;
            if (btnPlay.Checked)
            {
                if (gameRun())
                {
                    btnPause.Enabled = true;
                    btnBackground.Enabled = true;
                    btnDirectory.Enabled = false;
                }
                else
                {
                    btnPlay.Checked = false;
                }
            }
            else
            {
                gameBreak();
                btnPause.Enabled = false;
                btnBackground.Enabled = false;
                btnDirectory.Enabled = true;
            }
        }

        private void btnPause_CheckedChanged(object sender, EventArgs e)
        {
            if (btnPause.Checked)
            {
                GLES2View.Stop();
                btnStep.Enabled = true;
                btnBackground.Enabled = false;
            }
            else
            {
                btnStep.Enabled = false;
                GLES2View.Start();
                btnBackground.Enabled = true;
            }
        }

        private void btnStep_Click(object sender, EventArgs e)
        {
            GLES2View.Update();
        }

        private void btnBackground_CheckedChanged(object sender, EventArgs e)
        {
            if (btnBackground.Checked)
            {
                btnBackground.Text = "Background";
                btnPause.Enabled = false;
                gameToBackground();
            }
            else
            {
                btnBackground.Text = "Foreground";
                btnPause.Enabled = true;
                gameToForeground();
            }
        }

        private void logText_TextChanged(object sender, EventArgs e){}

        private void btnDirectory_Click(object sender, EventArgs e)
        {
            selectPath();
        }

        private void lblFPS_Click(object sender, EventArgs e)
        {

        }

    }
}
