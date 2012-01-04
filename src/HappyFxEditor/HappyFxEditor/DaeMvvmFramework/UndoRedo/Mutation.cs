using System;

namespace DaeMvvmFramework
{
    /// <summary>
    /// A Mutation encapsulates Undo and Redo methods.
    /// It also provides a Do method which is called
    /// the first time the Mutation is executed. 
    /// Typically Do and Redo share the same code.
    /// </summary>
    /// <remarks>
    /// Only the Evolution can call Do, Redo and Undo;
    /// hence these methods are marked internal protected.
    /// </remarks>
    public abstract class Mutation
    {
        protected internal abstract void Do();
        protected internal abstract void Redo();
        protected internal abstract void Undo();
    }
}