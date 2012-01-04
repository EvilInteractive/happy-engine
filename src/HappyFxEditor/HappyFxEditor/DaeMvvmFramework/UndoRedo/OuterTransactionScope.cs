using System.Collections.Generic;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Collects mutations during a transaction.
    /// </summary>
    internal sealed class OuterTransactionScope : TransactionScope
    {
        private readonly Evolution _evolution;
        private readonly List<Mutation> _mutations = new List<Mutation>();

        public OuterTransactionScope(Evolution evolution)
        {
            _evolution = evolution;
        }

        private Mutation Close()
        {
            switch( _mutations.Count )
            {
                case 0:
                    return null;
                case 1:
                    return _mutations[0];
                default:
                    return new TransactionMutation(_mutations);
            }
        }

        public override TransactionScope Parent
        {
            get { return null; }
        }

        public override void Add(Mutation mutation)
        {
            _mutations.Add(mutation);
        }

        public override void Do(Mutation mutation)
        {
            mutation.Do();
            Add(mutation);
        }

        public override void Revert()
        {
            _mutations.ForEach(mutation => mutation.Undo());
            _mutations.Clear();
        }

        public override void Dispose()
        {
            _evolution.EndTransaction(this, Close());
        }
    }
}