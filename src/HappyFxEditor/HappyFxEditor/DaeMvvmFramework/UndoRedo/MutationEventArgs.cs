using System;

namespace DaeMvvmFramework
{
    public sealed class MutationEventArgs : EventArgs
    {
        public readonly Mutation Mutation;
        public readonly MutationMode Mode;

        public MutationEventArgs(Mutation mutation, MutationMode mode)
        {
            Mutation = mutation;
            Mode = mode;
        }
    }
}