using System;

namespace DaeMvvmFramework
{
    /// <summary>
    /// The scope of a transaction of mutations
    /// </summary>
    internal abstract class TransactionScope : IDisposable
    {
        public abstract TransactionScope Parent { get; }
        public abstract void Add(Mutation mutation);
        public abstract void Do(Mutation mutation);
        public abstract void Revert();
        public abstract void Dispose();
    }
}