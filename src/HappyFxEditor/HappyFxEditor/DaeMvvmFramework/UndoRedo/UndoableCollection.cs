using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;

namespace DaeMvvmFramework
{
    public class UndoableCollection<T> : ObservableCollection<T>
    {
        private readonly UndoableChangeSource _owner;

        public UndoableCollection(UndoableChangeSource owner)
        {
            _owner = owner;
        }

        public UndoableCollection(UndoableChangeSource owner, IEnumerable<T> items):base(items)
        {
            _owner = owner;
        }

        public void AddRange(IEnumerable<T> items)
        {
            foreach (var item in items)
            {
                Add(item);
            }
        }

        protected override void ClearItems()
        {
            if( _owner.IsEvolving )
            {
                var oldItems = Items.ToArray();

                _owner.Evolution.Add(
                    MutationFactory.Execute(
                        () => base.ClearItems(),
                        () => AddRange(oldItems)));
            }

            base.ClearItems();
        }

        protected override void InsertItem(int index, T item)
        {
            if (_owner.IsEvolving)
            {
                _owner.Evolution.Add(
                    MutationFactory.Execute(
                        () => base.InsertItem(index, item),
                        () => RemoveAt(index)));
            }

            base.InsertItem(index, item);
        }

        protected override void RemoveItem(int index)
        {
            if (_owner.IsEvolving)
            {
                T oldItem = this[index];

                _owner.Evolution.Add(
                    MutationFactory.Execute(
                        () => RemoveAt(index),
                        () => base.InsertItem(index, oldItem)));
            }

            base.RemoveItem(index);
        }

        protected override void MoveItem(int oldIndex, int newIndex)
        {
            if (_owner.IsEvolving)
            {
                _owner.Evolution.Add(
                    MutationFactory.Execute(
                        () => base.MoveItem(oldIndex, newIndex),
                        () => base.MoveItem(newIndex, oldIndex)));
            }

            base.MoveItem(oldIndex, newIndex);
        }

        protected override void SetItem(int index, T item)
        {
            if (_owner.IsEvolving)
            {
                T oldItem = this[index];

                _owner.Evolution.Add(
                    MutationFactory.Execute(
                        () => base.SetItem(index, item),
                        () => base.SetItem(index, oldItem)));
            }

            base.SetItem(index, item);
        }
    }
}