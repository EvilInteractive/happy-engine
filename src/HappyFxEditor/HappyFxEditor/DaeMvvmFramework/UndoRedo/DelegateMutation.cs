using System;

namespace DaeMvvmFramework
{
    public sealed class DelegateMutation : Mutation
    {
        private readonly Action _do;
        private readonly Action _undo;
        private readonly Action _redo;

        public DelegateMutation(Action @do, Action redo, Action undo)
        {
            _do = @do;
            _undo = undo;
            _redo = redo;
        }

        public DelegateMutation(Action redo, Action undo):this(redo,redo,undo)
        {
        }

        public DelegateMutation(Action swap):this(swap,swap,swap)
        {
        }

        protected internal override void Do()
        {
            _do();
        }

        protected internal override void Redo()
        {
            _redo();
        }

        protected internal override void Undo()
        {
            _undo();
        }
    }
}