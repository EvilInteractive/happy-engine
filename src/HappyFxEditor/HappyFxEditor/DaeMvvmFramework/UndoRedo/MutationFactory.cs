using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Data;

namespace DaeMvvmFramework
{
    public static class MutationFactory
    {
        public static SwapMutation<T> Swap<T>(Func<T, T> swapper, T newValue)
        {
            return new SwapMutation<T>(swapper, newValue);
        }

        public static DelegateMutation Execute(Action @do, Action redo, Action undo)
        {
            return new DelegateMutation(@do, redo, undo);
        }

        public static DelegateMutation Execute(Action redo, Action undo)
        {
            return new DelegateMutation(redo, redo, undo);
        }

        public static DelegateMutation Execute(Action swap)
        {
            return new DelegateMutation(swap, swap, swap);
        }
    }
}