using System;
using System.ComponentModel;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Handy class to implement a WPF command 
    /// that takes in a command parameter using delegates.
    /// 
    /// Also supports listening to property changes that are used
    /// to determine if the command can be executed.
    /// </summary>
    /// <typeparam name="T">
    /// The type of the parameter passed to the Execute and CanExecute methods.
    /// </typeparam>
    public class DelegateCommand<T> : CommandBase
    {
        private readonly Action<T> _execute;
        private readonly Func<T, bool> _canExecute;

        public DelegateCommand(Action<T> execute)
        {
            _execute = execute;
        }

        public DelegateCommand(Action<T> execute, Func<T,bool> canExecute, 
                               INotifyPropertyChanged context=null, params string[] canExecutePropertyNames) : base(context, canExecutePropertyNames)
        {
            _execute = execute;
            _canExecute = canExecute;
        }

        public override void Execute(object parameter)
        {
            _execute((T) parameter);
        }

        public override bool CanExecute(object parameter)
        {
            return _canExecute == null || _canExecute((T) parameter);
        }
    }
}