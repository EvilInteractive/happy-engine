using System;

namespace DaeMvvmFramework
{
    internal sealed class InnerTransactionScope : TransactionScope
    {
        private readonly TransactionScope _parent;

        public InnerTransactionScope(TransactionScope parent)
        {
            _parent = parent;
        }

        public override TransactionScope Parent
        {
            get { return _parent; }
        }

        public override void Add(Mutation mutation)
        {
            _parent.Add(mutation);
        }

        public override void Do(Mutation mutation)
        {
            _parent.Do(mutation);
        }

        public override void Revert()
        {
            _parent.Revert();
        }

        public override void Dispose()
        {
        }
    }
}