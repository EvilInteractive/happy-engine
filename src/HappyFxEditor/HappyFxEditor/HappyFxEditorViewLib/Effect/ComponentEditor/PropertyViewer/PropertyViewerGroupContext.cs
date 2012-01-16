using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DaeMvvmFramework;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer
{
    public class PropertyViewerGroupContext : PropertyChangeSource
    {
        public PropertyViewerContext PropertyViewer { get; private set; }

        public string Name { get; private set; }

        public List<PropertyViewerItemContext> Items { get; private set; }

        #region SelectedItem
        private PropertyViewerItemContext _selectedItem;
        public const string SelectedItemProperty = "SelectedItem";
        public PropertyViewerItemContext SelectedItem
        {
            get { return _selectedItem; }
            set { Change(_selectedItem, value, (newValue) =>
                                                   {
                                                       _selectedItem = newValue;
                                                       PropertyViewer.SelectedGroup = this;
                                                   }, SelectedItemProperty); }
        }
        #endregion
        

        public PropertyViewerGroupContext(PropertyViewerContext propertyViewer, string name)
        {
            PropertyViewer = propertyViewer;
            Items = new List<PropertyViewerItemContext>();
            Name = name;
        }

        internal PropertyViewerGroupContext Copy(PropertyViewerContext property)
        {
            PropertyViewerGroupContext group = new PropertyViewerGroupContext(property, Name);
            foreach (var item in Items)
            {
                group.Items.Add(item.Copy(group));
            }

            return group;
        }
    }
}
