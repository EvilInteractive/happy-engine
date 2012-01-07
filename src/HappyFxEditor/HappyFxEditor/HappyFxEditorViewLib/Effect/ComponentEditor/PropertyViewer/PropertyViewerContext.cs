using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer
{
    public class PropertyViewerContext
    {
        public EffectContext Effect { get; private set; }

        public List<PropertyViewerGroupContext> Groups { get; private set; }

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
