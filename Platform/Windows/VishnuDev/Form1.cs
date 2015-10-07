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
        private bool is_VR = false;
        private Timer watchTimer = null;
        private VishnuGLES2 dispTarget = null;

        public Form1()
        {
            InitializeComponent();
            dispTarget = GLES2View;

            //コンボボックスにディスプレイのリストを表示する
            this.comboDisplay.DropDownStyle = ComboBoxStyle.DropDownList;
            //デバイス名が表示されるようにする
            this.comboDisplay.DisplayMember = "DeviceName";
            this.comboDisplay.DataSource = Screen.AllScreens;
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
            double fps = dispTarget.nowFps;
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
            dispTarget.RestoreGLES2();
            VishnuSystem.Instance.Start(is_VR);
            dispTarget.Start();
            running = true;
            chkVREnv.Enabled = false;
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
                dispTarget.CreateSurface();
                background = false;
            }
            dispTarget.Stop();
            watchTimer.Stop();
            lblFPS.Text = "";
            VishnuSystem.Instance.Stop();
            dispTarget.DestroyGLES2();
            chkVREnv.Enabled = true;
            running = false;
        }

        private void gameToBackground()
        {
            if (background) return;
            dispTarget.Stop();
            VishnuSystem.Instance.OnPause();
            dispTarget.DestroySurface();
            background = true;
        }

        private void gameToForeground()
        {
            if (!background) return;
            dispTarget.CreateSurface();
            VishnuSystem.Instance.OnResume();
            dispTarget.Start();
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
                dispTarget.Stop();
                btnStep.Enabled = true;
                btnBackground.Enabled = false;
            }
            else
            {
                btnStep.Enabled = false;
                dispTarget.Start();
                btnBackground.Enabled = true;
            }
        }

        private void btnStep_Click(object sender, EventArgs e)
        {
            dispTarget.Update();
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

        private FormVR formVR = null;
        private bool isVR = false;
        private void VRDisplayActive(bool active)
        {
            bool is_background = background;
            if (!isVR && active)
            {
                if (running && !is_background) gameToBackground();
                formVR = new FormVR();
                Screen s = (Screen)comboDisplay.SelectedItem;
                formVR.StartPosition = FormStartPosition.Manual;
                formVR.Location = s.Bounds.Location;
                formVR.Show();
                dispTarget = formVR.getGLES2Control();
                GLES2View.Hide();
                if (running && !is_background) gameToForeground();
                isVR = true;
                return;
            }
            if (isVR && !active)
            {
                if (running && !is_background) gameToBackground();
                formVR.Hide();
                formVR = null;
                GLES2View.Show();
                dispTarget = GLES2View;
                if (running && !is_background) gameToForeground();
                isVR = false;
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox chkBox = (CheckBox)sender;
            if (chkBox.Checked)
            {
                chkBox.Text = "to Preview";
                VRDisplayActive(true);
            }
            else
            {
                chkBox.Text = "to VR";
                VRDisplayActive(false);
            }

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            gameBreak();
        }

        private void chkVREnv_CheckedChanged(object sender, EventArgs e)
        {

            CheckBox chkBox = (CheckBox)sender;
            if (chkBox.Checked)
            {
                chkDisplay.Enabled = true;
                comboDisplay.Enabled = true;
                is_VR = true;
            }
            else
            {
                chkDisplay.Enabled = false;
                comboDisplay.Enabled = false;
                is_VR = false;
            }
        }


    }
}
