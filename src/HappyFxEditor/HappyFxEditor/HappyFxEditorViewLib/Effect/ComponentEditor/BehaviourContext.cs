using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;

namespace HappyFxEditorContextLib.Effect.ComponentEditor
{
    public class BehaviourContext : PropertyChangeSource
    {
        private int _type = 0;

        #region Name
        private string _name;
        public const string NameProperty = "Name";
        public string Name
        {
            get { return _name; }
            private set { Change(ref _name, value, NameProperty); }
        }
        #endregion
        
        #region Color
        private Brush _color;
        public const string ColorProperty = "Color";
        public Brush Color
        {
            get { return _color; }
            private set { Change(ref _color, value, ColorProperty); }
        }
        #endregion

        #region Hue
        private short _hue = 0;
        public short Hue
        {
            get { return _hue; }
            set
            {
                _hue = value;
                UpdateColors();
            }
        }
        #endregion

        public PropertyViewerContext Properties { get; private set; }

        public string ToolTip { get; set; }

        public BehaviourContext(PropertyViewerContext properties)
        {
            Properties = properties;
        }

        public void SetType<T>(T type, string displayName)
        {
            _type = (int)(object)type;
            Name = displayName;
        }
        public T GetType<T>()
        {
            return (T)(object)_type;
        }

        private void UpdateColors()
        {
            Color = new SolidColorBrush(ColorExtensions.FromHsb(Hue, 0.89, 0.37));
        }

        internal BehaviourContext Copy()
        {
            return new BehaviourContext(Properties)
                       {
                           Name = this.Name,
                           Color = this.Color,
                           Hue = this.Hue,
                           _type = this._type
                       };
        }
    }
}
