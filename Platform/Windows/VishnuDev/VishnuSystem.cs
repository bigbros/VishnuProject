using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace VishnuDev
{
    class VishnuSystem {

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr vsnCreate(string path, int width, int height, int is_vr);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnDestroy(IntPtr vsn);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnUpdate(IntPtr vsn);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnOnPointing(IntPtr vsn, int idx, int mode, int x, int y, float force);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnOnSizeChanged(IntPtr vsn, int width, int height);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnOnDetach(IntPtr vsn);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnOnSurface(IntPtr vsn);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnOnPause(IntPtr vsn);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnOnResume(IntPtr vsn);

        [DllImport("VishnuDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void vsnSetLogFunc(LogDisplay func);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void LogDisplay(string msg);

        private string pathBase = null;
        public string Path {
            get{
                return pathBase;
            }
            set{
                pathBase = value;
            }
        }

        private IntPtr vsnInstance = IntPtr.Zero;
        private LogDisplay logDisp = null;

        private static VishnuSystem instance;
        public static VishnuSystem Instance
        {
            get
            {
                if (instance == null) instance = new VishnuSystem();
                return instance;
            }
        }

        private VishnuSystem()
        {
        }

        public void setLogMethod(LogDisplay method)
        {
            logDisp = method;
            vsnSetLogFunc(logDisp);
        }

        public void setPath(string path)
        {
            pathBase = path;
        }

        public void Start(string path, bool isvr = false)
        {
            pathBase = path;
            Start(isvr);
        }

        public void Start(bool isvr = false)
        {
            EventGLES2.Instance.onSurface += OnSurface;
            EventGLES2.Instance.onDetach += OnDetach;
            EventGLES2.Instance.onPointing += OnPointing;
            EventGLES2.Instance.onSizeChanged += OnSizeChanged;
            EventGLES2.Instance.update += Update;
            vsnInstance = vsnCreate(pathBase, 100, 100, ((isvr) ? 1 : 0));
        }

        public void Stop()
        {
            vsnDestroy(vsnInstance);
            EventGLES2.Instance.onSurface -= OnSurface;
            EventGLES2.Instance.onDetach -= OnDetach;
            EventGLES2.Instance.onPointing -= OnPointing;
            EventGLES2.Instance.onSizeChanged -= OnSizeChanged;
            EventGLES2.Instance.update -= Update;
            vsnInstance = IntPtr.Zero;
        }

        public void Restart()
        {
            Stop();
            Start();
        }

        public void OnPause()
        {
            if(vsnInstance != IntPtr.Zero) vsnOnPause(vsnInstance);
        }

        public void OnResume()
        {
            if (vsnInstance != IntPtr.Zero) vsnOnResume(vsnInstance);
        }

        private void OnSurface()
        {
            if (vsnInstance != IntPtr.Zero) vsnOnSurface(vsnInstance);
        }

        private void OnDetach()
        {
            if (vsnInstance != IntPtr.Zero) vsnOnDetach(vsnInstance);
        }

        private void OnPointing(int id, int mode, int x, int y, float force)
        {
            if (vsnInstance != IntPtr.Zero) vsnOnPointing(vsnInstance, id, mode, x, y, force);
        }

        private void OnSizeChanged(int width, int height)
        {
            if (vsnInstance != IntPtr.Zero) vsnOnSizeChanged(vsnInstance, width, height);
        }

        private void Update()
        {
            if (vsnInstance != IntPtr.Zero) vsnUpdate(vsnInstance);
        }
    }
}
