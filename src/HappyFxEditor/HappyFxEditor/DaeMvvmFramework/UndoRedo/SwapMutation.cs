using System;

namespace DaeMvvmFramework
{
    /// <summary>
    /// A mutation that swaps a value using a delegate.
    /// </summary>
    public class SwapMutation<T> : Mutation 
    {
        private readonly Func<T, T> _swapper;
        private T _value;

        public SwapMutation(Func<T,T> swapper, T newValue)
        {
            _swapper = swapper;
            _value = newValue;
        }

        protected internal override void Do()
        {
            Swap();
        }

        protected internal override void Undo()
        {
            Swap();
        }

        protected internal override void Redo()
        {
            Swap();
        }

        private void Swap()
        {
            _value = _swapper(_value);
        }
    }
}