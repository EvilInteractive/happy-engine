using System;
using System.ComponentModel;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Handy class to implement a parameterless WPF command using delegates.
    /// 
    /// Also supports listening to property changes that are used
    /// to determine if the command can be executed.
    /// </summary>
    public class DelegateCommand : CommandBase
    {
        private readonly Action _execute;
        private readonly Func<bool> _canExecute;

        public DelegateCommand(Action execute)
        {
            _execute = execute;
        }

        public DelegateCommand(Action execute, Func<bool> canExecute,
                               INotifyPropertyChanged context = null, params string[] canExecutePropertyNames)
            : base(context, canExecutePropertyNames)
        {
            _execute = execute;
            _canExecute = canExecute;
        }

        public override void Execute(object parameter)
        {
            _execute();
        }

        public override bool CanExecute(object parameter)
        {
            return _canExecute == null || _canExecute();
        }
    }
}