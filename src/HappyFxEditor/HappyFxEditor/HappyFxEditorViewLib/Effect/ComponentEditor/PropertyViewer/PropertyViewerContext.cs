using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DaeMvvmFramework;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer
{
    public class PropertyViewerContext : PropertyChangeSource
    {
        public EffectContext Effect { get; private set; }

        public List<PropertyViewerGroupContext> Groups { get; private set; }

        #region SelectedGroup
        private PropertyViewerGroupContext _selectedGroup;
        public const string SelectedGroupProperty = "SelectedGroup";
        public PropertyViewerGroupContext SelectedGroup
        {
            get { return _selectedGroup; }
            set { Change(ref _selectedGroup, value, SelectedGroupProperty); }
        }
        #endregion       

        public PropertyViewerContext(EffectContext effect)
        {
            Effect = effect;
            Groups = new List<PropertyViewerGroupContext>();
        }

        public PropertyViewerContext Copy()
        {
            var temp = new PropertyViewerContext(Effect);
            foreach (var group in Groups)
            {
                temp.Groups.Add(group.Copy(temp));
            }
            return temp;
        }
    }
}
