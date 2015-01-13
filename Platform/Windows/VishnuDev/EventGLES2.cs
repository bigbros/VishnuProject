using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VishnuDev
{
    class EventGLES2
    {
        static private EventGLES2 instance = null;
        static public EventGLES2 Instance
        {
            get {
                if (instance == null) instance = new EventGLES2();
                return instance;
            }
        }
        public delegate void EvOnPointing(int id, int mode, int x, int y, float force);
        public delegate void EvOnSizeChanged(int width, int height);

        public delegate void EvOnDetach();
        public delegate void EvOnSurface();
        
        public delegate void EvUpdate();

        public event EvOnPointing onPointing;
        public event EvOnSizeChanged onSizeChanged;
        public event EvUpdate update;
        public event EvOnDetach onDetach;
        public event EvOnSurface onSurface;

        private EventGLES2() {}

        public void OnPointing(int id, int mode, int x, int y, float force)
        {
            if (onPointing != null) onPointing(id, mode, x, y, force);
        }

        public void OnSizeChanged(int width, int height)
        {
            if (onSizeChanged != null) onSizeChanged(width, height);
        }

        public void OnDetach()
        {
            if (onDetach != null) onDetach();
        }

        public void OnSurface()
        {
            if (onSurface != null) onSurface();
        }

        public void Update()
        {
            if (update != null) update();
        }
    }
}
