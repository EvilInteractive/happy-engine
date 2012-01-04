using System;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Handy base class to raise property change events,
    /// and record mutations for each change.
    /// </summary>
    public abstract class UndoableChangeSource : PropertyChangeSource
    {
        public abstract Evolution Evolution { get; }

        public void Do(Mutation mutation)
        {
            var evolution = Evolution;

            if( evolution==null )
            {
                mutation.Do();
            }
            else
            {
                evolution.Do(mutation);
            }
        }

        public bool IsEvolving
        {
            get
            {
                var evolution = Evolution;
                return evolution != null && evolution.Mode == MutationMode.Doing;
            }
        }

        public IDisposable BeginTransaction()
        {
            var evolution = Evolution;
            return evolution == null ? new DirectTransactionScope() : evolution.BeginTransaction();
        }

        protected T Swap<T>(ref T field, T newValue, string propertyName, params string[] dependentPropertyNames)
        {
            T oldValue = field;
            field = newValue;
            OnPropertyChanged(propertyName, dependentPropertyNames);
            return oldValue;
        }

        protected T Swap<T>(T newValue, Func<T> getter, Action<T> setter, string propertyName, params string[] dependentPropertyNames)
        {
            T oldValue = getter();
            setter(newValue);
            OnPropertyChanged(propertyName, dependentPropertyNames);
            return oldValue;
        }

        /// <summary>
        /// Undoable change
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="oldValue"></param>
        /// <param name="newValue"></param>
        /// <param name="swapper"></param>
        /// <returns></returns>
        protected virtual bool Change<T>(T oldValue, T newValue, Func<T,T> swapper)
        {
            bool isChange = !Equals(oldValue, newValue);

            if (isChange)
            {
                if( IsEvolving )
                {
                    Evolution.Do(new SwapMutation<T>(swapper, newValue));
                }
                else
                {
                    swapper(newValue);
                }
            }

            return isChange;
        }
    }
}