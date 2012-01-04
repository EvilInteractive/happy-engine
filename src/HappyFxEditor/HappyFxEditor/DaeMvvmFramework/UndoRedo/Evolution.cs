using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace DaeMvvmFramework
{
    /// <summary>
    /// A Evolution keeps two stacks of mutations,
    /// one for the mutations that can be undone,
    /// one for the mutations that can be redone.
    /// </summary>
    /// <remarks>
    /// Doing a new mutation clears the redo stack.
    /// (a different implementation might keep all muations, see e.g. Emacs)
    /// </remarks>
    public class Evolution : PropertyChangeSource
    {
        private sealed class MutationStack : List<Mutation>
        {
            public Mutation Pop()
            {
                Mutation result = this[Count - 1];
                RemoveAt(Count - 1);
                return result;
            }

            public void Push(Mutation mutation)
            {
                Add(mutation);
            }
        }

        private readonly int _maxSize;
        private MutationMode _mode;
        private TransactionScope _scope;

        private readonly MutationStack _undoables = new MutationStack();
        private readonly MutationStack _redoables = new MutationStack();

        /// <summary>
        /// Raised whenever Undo, Redo or Do is called.
        /// </summary>
        public event EventHandler<MutationEventArgs> Changed;

        public Evolution(int maxSize)
        {
            _maxSize = maxSize;
        }

        public Evolution():this(int.MaxValue)
        {
        }

        public const string ModeProperty = "Mode";
        public const string CanUndoProperty = "CanUndo";
        public const string CanRedoProperty = "CanRedo";

        public bool CanUndo
        {
            get { return _undoables.Count > 0;  }
        }

        public bool CanRedo
        {
            get { return _redoables.Count > 0; }
        }

        public MutationMode Mode
        {
            get { return _mode; }
            private set { Change(ref _mode, value, ModeProperty); }
        }

        public IDisposable BeginTransaction()
        {
            if( Mode != MutationMode.Doing )
                throw new InvalidOperationException("BeginTransaction cannot be called while undoing/redoing");

            if( _scope == null )
            {
                _scope = new OuterTransactionScope(this);
            }
            else
            {
                _scope = new InnerTransactionScope(_scope);
            }

            return _scope;
        }

        internal void EndTransaction(OuterTransactionScope scope, Mutation mutation)
        {
            EndTransaction(scope);
            Add(mutation);
        }

        internal void EndTransaction(TransactionScope scope)
        {
            if( _scope != scope )
                throw new ArgumentException("scope");

            _scope = scope.Parent;
        }

        public void Undo()
        {
            Debug.Assert(CanUndo);
            Debug.Assert(Mode == MutationMode.Doing);

            try
            {
                Mode = MutationMode.Undoing;
                Mutation mutation = _undoables.Pop();
                mutation.Undo();
                _redoables.Push(mutation);
                OnChanged(mutation, MutationMode.Undoing);
            }
            finally
            {
                Mode = MutationMode.Doing;
            }
        }

        public void Redo()
        {
            Debug.Assert(CanRedo);
            Debug.Assert(Mode == MutationMode.Doing);

            try
            {
                Mode = MutationMode.Redoing;
                Mutation mutation = _redoables.Pop();
                mutation.Redo();
                _undoables.Push(mutation);
                OnChanged(mutation, MutationMode.Redoing);
            }
            finally
            {
                Mode = MutationMode.Doing;
            }
        }

        public void Do(Mutation mutation)
        {
            if (mutation != null)
            {
                if (Mode != MutationMode.Doing)
                    throw new InvalidOperationException("Do cannot be called while undoing/redoing");

                if (_scope != null)
                {
                    _scope.Do(mutation);
                }
                else
                {
                    mutation.Do();
                    Add(mutation);
                }
            }
        }

        /// <summary>
        /// Adds a mutation without executing it.
        /// </summary>
        /// <param name="mutation"></param>
        internal void Add(Mutation mutation)
        {
            if (mutation != null)
            {
                if (_scope != null)
                {
                    _scope.Add(mutation);
                }
                else
                {
                    _redoables.Clear();

                    if (_undoables.Count >= _maxSize)
                    {
                        _undoables.RemoveAt(0);
                    }

                    _undoables.Push(mutation);

                    OnChanged(mutation, MutationMode.Doing);
                }
            }
        }

        public void Clear()
        {
            _undoables.Clear();
            _redoables.Clear();
            OnPropertyChanged(CanUndoProperty, CanRedoProperty);
        }

        protected virtual void OnChanged(Mutation mutation, MutationMode kind)
        {
            OnPropertyChanged(CanUndoProperty, CanRedoProperty);

            var handler = Changed;
            if (handler != null)
                handler(this, new MutationEventArgs(mutation, kind));
        }

        public void Dispose()
        {
        }
    }
}
