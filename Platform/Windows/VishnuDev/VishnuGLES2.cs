using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
// using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;
using System.IO;

namespace VishnuDev
{
    public partial class VishnuGLES2 : UserControl
    {
        [DllImport("kernel32.dll")]
        static extern bool QueryPerformanceCounter(ref long lpPerformanceCount);
  
        [DllImport("kernel32.dll")]
        static extern bool QueryPerformanceFrequency(ref long lpFrequency);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr gles2CreateWindow(IntPtr hWnd);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void gles2DestroyWindow(IntPtr gles2);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void gles2Resize(IntPtr gles2, int width, int height);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void gles2Update(IntPtr gles2);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void gles2Flip(IntPtr gles2);

        private IntPtr envGLES2 = IntPtr.Zero;

        private bool sizeChanged = false;
        private bool isMouseDown = false;
        private Timer updateTimer = null;

        public VishnuGLES2()
        {
            InitializeComponent();
            try
            {
                this.Disposed += OnUnload;
            }
            catch {
                envGLES2 = IntPtr.Zero;
            }
        }

        public void CreateSurface()
        {
            RestoreGLES2();
            EventGLES2.Instance.OnSurface();
        }

        public void DestroySurface()
        {
            EventGLES2.Instance.OnDetach();
            DestroyGLES2();
        }

        public void DestroyGLES2()
        {
            if (envGLES2 != IntPtr.Zero)
            {
                gles2DestroyWindow(envGLES2);
                envGLES2 = IntPtr.Zero;
                this.Invalidate();
            }
        }

        public void RestoreGLES2()
        {
            Module[] ms = Assembly.GetEntryAssembly().GetModules();
            IntPtr hInstance = Marshal.GetHINSTANCE(ms[0]);
            envGLES2 = gles2CreateWindow(this.Handle);
        }

        // サイズが変更された際の処理: GLES2の子Windowsのサイズも同じサイズに変更
        protected override void OnResize(EventArgs args)
        {
            base.OnResize(args);
            if (envGLES2 == IntPtr.Zero) return;
            gles2Resize(envGLES2, this.Width, this.Height);
            sizeChanged = true;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (envGLES2 == IntPtr.Zero) return;
            EventGLES2.Instance.OnPointing(0, 0, e.X, e.Y, 1.0f);
            isMouseDown = true;
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            if (envGLES2 == IntPtr.Zero) return;
            EventGLES2.Instance.OnPointing(0, 2, e.X, e.Y, 1.0f);
            isMouseDown = false;
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            if (!isMouseDown || envGLES2 == IntPtr.Zero) return;
            EventGLES2.Instance.OnPointing(0, 1, e.X, e.Y, 1.0f);
        }

        protected void OnUnload(Object sender, EventArgs args)
        {
            DestroySurface();
        }

        private long start = 0;
        private long freq = 0;
        private double fps = 0;
        public double nowFps {
            get { return fps; }
        }
        protected void OnUpdate(Object sender, EventArgs args)
        {
            bool disp = (freq > 0);
            long stop = 0;
            QueryPerformanceCounter(ref stop);
            freq = 0;
            QueryPerformanceFrequency(ref freq);
            Update();
            fps = (disp) ? (1000.0 / ((stop - start) * 1000.0 / freq)) : 0.0;
            start = stop;
        }

        public void Update()
        {
            if (envGLES2 == IntPtr.Zero) return;
            if (sizeChanged)
            {
                EventGLES2.Instance.OnSizeChanged(this.Width, this.Height);
                sizeChanged = false;
            }
            gles2Update(envGLES2);
            EventGLES2.Instance.Update();
            gles2Flip(envGLES2);
        }

        public void Start()
        {
            if (null == updateTimer)
            {
                updateTimer = new Timer();
                updateTimer.Tick += OnUpdate;
            }
            sizeChanged = true;
            updateTimer.Interval = 5;
            freq = 0L;
            fps = 0L;
            updateTimer.Start();
            QueryPerformanceCounter(ref start);
        }

        public void Stop()
        {
            updateTimer.Stop();
            freq = 0L;
            fps = 0L;
        }
    }
}
