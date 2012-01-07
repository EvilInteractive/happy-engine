using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace HappyFxEditorApp.Effect.ComponentEditor
{
    /// <summary>
    /// Interaction logic for ComponentToolBox.xaml
    /// </summary>
    public partial class ComponentToolBox : UserControl
    {
        public ComponentToolBox()
        {
            InitializeComponent();
        }

            
        public ICommand AddBehaviourCommand
        {
            get { return (ICommand)GetValue(AddBehaviourCommandProperty); }
            set { SetValue(AddBehaviourCommandProperty, value); }
        }
        public static readonly DependencyProperty AddBehaviourCommandProperty =
            DependencyProperty.Register("AddBehaviourCommand", typeof(ICommand), typeof(ComponentToolBox));

        private void ToolBoxDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (AddBehaviourCommand != null)
            {
                if (AddBehaviourCommand.CanExecute(null))
                    AddBehaviourCommand.Execute(null);
            }
        }
    }
}
