using System;
using System.ComponentModel;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Handy base class to raise property change events.
    /// </summary>
    public abstract class PropertyChangeSource : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Raise one or more property changed events.
        /// </summary>
        /// <remarks>
        /// Sometimes when one property changes, other dependent properties also change.
        /// For example, if you have a Person class with a FirstName and Name property,
        /// and a get-only FullName property that returns FirstName + " " + Name.
        /// When either FirstName or Name changes, FullName also changes,
        /// so observers (e.g. bound views) need to be notified of this.
        /// </remarks>
        protected virtual void OnPropertyChanged(string propertyName, params string[] dependentPropertyNames)
        {
            var handler = PropertyChanged;

            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(propertyName));

                foreach (var dependentPropertyName in dependentPropertyNames)
                {
                    handler(this, new PropertyChangedEventArgs(dependentPropertyName));
                }
            }
        }

        /// <summary>
        /// Changes a field to a new value, and raises one or more property changes events if needed.
        /// </summary>
        /// <typeparam name="T">
        /// The type of the backing field of the property
        /// </typeparam>
        /// <param name="backingField">
        /// The reference to backing field of the property
        /// </param>
        /// <param name="newValue">
        /// The new value to be assigned to the backing field.
        /// </param>
        /// <param name="propertyName">
        /// The name of the property
        /// </param>
        /// <param name="dependentPropertyNames">
        /// Zero or more names of properties that depent on the property.
        /// </param>
        /// <returns>
        /// true if the new value was different from the old value
        /// </returns>
        protected virtual bool Change<T>(ref T backingField, T newValue, string propertyName, params string[] dependentPropertyNames)
        {
            bool isChange = !Equals(backingField, newValue);

            if (isChange)
            {
                backingField = newValue;
                OnPropertyChanged(propertyName, dependentPropertyNames);
            }

            return isChange;
        }

        protected virtual bool Change<T>(T oldValue, T newValue, Action<T> setValue, string propertyName, params string[] dependentPropertyNames)
        {
            bool isChange = !Equals(oldValue, newValue);

            if (isChange)
            {
                setValue(newValue);
                OnPropertyChanged(propertyName, dependentPropertyNames);
            }

            return isChange;
        }

    }
}