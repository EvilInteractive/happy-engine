using System;

namespace DaeMvvmFramework
{
    internal sealed class DirectTransactionScope : TransactionScope
    {
        public override TransactionScope Parent
        {
            get { return null; }
        }

        public override void Add(Mutation mutation)
        {
        }

        public override void Do(Mutation mutation)
        {
            mutation.Do();
        }

        public override void Revert()
        {
            throw new InvalidOperationException();
        }

        public override void Dispose()
        {
        }
    }
}