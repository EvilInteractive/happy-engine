using System;
using System.ComponentModel;
using System.Linq;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Helper class to observer and act on one or more properties changes.
    /// </summary>
    public abstract class PropertyChangeObserver : IDisposable
    {
        private readonly string[] _propertyNames;
        private INotifyPropertyChanged _context;

        protected PropertyChangeObserver(INotifyPropertyChanged context=null, params string[] propertyNames)
        {
            _context = context;
            _propertyNames = propertyNames;

            if (_context != null && _propertyNames.Length > 0)
            {
                _context.PropertyChanged += HandleContextPropertyChange;
            }
        }

        private void  HandleContextPropertyChange(object sender, PropertyChangedEventArgs e)
        {
            if( _propertyNames.Any(name => name == e.PropertyName) )
            {
                OnContextPropertyChanged(e);
            }
        }

        protected abstract void OnContextPropertyChanged(PropertyChangedEventArgs e);

        public void Dispose()
        {
            if (_context != null && _propertyNames.Length > 0)
            {
                _context.PropertyChanged -= HandleContextPropertyChange;
                _context = null;
            }
        }
    }
}