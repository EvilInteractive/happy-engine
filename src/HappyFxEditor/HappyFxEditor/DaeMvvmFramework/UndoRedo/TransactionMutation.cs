using System.Collections.Generic;
using System.Linq;

namespace DaeMvvmFramework
{
    /// <summary>
    /// Multiple mutations grouped together as one transaction.
    /// </summary>
    internal sealed class TransactionMutation : Mutation
    {
        private readonly Mutation[] _mutations;

        public TransactionMutation(IEnumerable<Mutation> mutations)
        {
            _mutations = mutations.ToArray();
        }

        protected internal override void Do()
        {
            // Commands are already done.
        }

        protected internal override void Redo()
        {
            for (int i = 0; i < _mutations.Length; i++)
            {
                _mutations[i].Redo();
            }
        }

        protected internal override void Undo()
        {
            for (int i = _mutations.Length; --i >= 0; )
            {
                _mutations[i].Undo();
            }
        }
    }
}