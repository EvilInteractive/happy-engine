using System;
using System.ComponentModel;
using System.Windows.Input;

namespace DaeMvvmFramework
{
    public abstract class CommandBase : PropertyChangeObserver, ICommand
    {
        protected CommandBase(INotifyPropertyChanged context=null, params string[] propertyNames) : base(context, propertyNames)
        {
        }

        public virtual event EventHandler CanExecuteChanged;

        public abstract void Execute(object parameter);

        public abstract bool CanExecute(object parameter);

        private void OnCanExecuteChanged(EventArgs e)
        {
            EventHandler handler = CanExecuteChanged;
            if (handler != null) 
                handler(this, e);
        }

        protected override void OnContextPropertyChanged(PropertyChangedEventArgs e)
        {
            RaiseCanExecuteChanged();
        }

        public void RaiseCanExecuteChanged()
        {
            OnCanExecuteChanged(EventArgs.Empty);
        }
    }
}