using System;
using System.ComponentModel;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Handly helper class to create commands.
    /// </summary>
    public static class CommandFactory
    {
        public static DelegateCommand Create(Action execute)
        {
            return new DelegateCommand(execute);
        }

        public static DelegateCommand Create(Action execute, Func<bool> canExecute)
        {
            return new DelegateCommand(execute, canExecute);
        }

        public static DelegateCommand Create(Action execute, Func<bool> canExecute, 
            INotifyPropertyChanged context, params string[] propertyNames)
        {
            return new DelegateCommand(execute, canExecute, context, propertyNames);
        }

        public static DelegateCommand<T> Create<T>(Action<T> execute)
        {
            return new DelegateCommand<T>(execute);
        }

        public static DelegateCommand<T> Create<T>(Action<T> execute, Func<T, bool> canExecute)
        {
            return new DelegateCommand<T>(execute, canExecute);
        }

        public static DelegateCommand<T> Create<T>(Action<T> execute, Func<T, bool> canExecute,
            INotifyPropertyChanged context, params string[] propertyNames)
        {
            return new DelegateCommand<T>(execute, canExecute, context, propertyNames);
        }
    }
}