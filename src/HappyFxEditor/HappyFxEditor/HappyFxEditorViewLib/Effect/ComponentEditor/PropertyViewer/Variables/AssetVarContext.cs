using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Input;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Browsers;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    class AssetVarContext : UndoableChangeSource, IVariableContext
    {
        #region Asset
        private Asset _asset;
        public const string AssetNameProperty = "Asset";
        public Asset Asset
        {
            get { return _asset; }
            set { Change(_asset, value, (newValue) => Swap(ref _asset, newValue, AssetNameProperty)); }
        }
        #endregion

        public ICommand AssignCommand { get; private set; }

        public AssetType Type { get; private set; }

        private readonly EffectContext _effect;

        public AssetVarContext(EffectContext effect, VariableComponentType type)
        {
            _effect = effect;
            switch (type)
            {
                case VariableComponentType.Texture:
                    Init(AssetType.Texture);
                    break;
                default:
                    throw new ArgumentOutOfRangeException("type");
            }
        }

        public AssetVarContext(EffectContext effect, AssetType type)
        {
            _effect = effect;
            Init(type);
        }
        private void Init(AssetType type)
        {
            Type = type;
            AssignCommand = CommandFactory.Create(() => { Asset = _effect.MainContext.AssetManager.SelectedAsset; },
                                                  () => _effect.MainContext.AssetManager.SelectedAsset != null &&
                                                        _effect.MainContext.AssetManager.SelectedAsset.Type == Type,
                                                        _effect.MainContext.AssetManager,
                                                        AssetManagerContext.SelectedAssetProperty);           
        }

        public VariableType GetVarType()
        {
            return VariableType.Asset;
        }

        public IVariableContext Copy(EffectContext effect)
        {
            AssetVarContext var = new AssetVarContext(effect, Type) { Asset = this.Asset };
            return var;
        }

        public void Serialize(BinaryWriter stream)
        {
            string path = "";
            if (Asset != null)
                path = Asset.Path;
            stream.Write((UInt32)path.Length);
            stream.Write(Encoding.ASCII.GetBytes(path));
        }

        public void DeSerialize(BinaryReader stream)
        {
            uint len = stream.ReadUInt32();
            string path = Encoding.ASCII.GetString(stream.ReadBytes((int)len));
            Asset = new Asset(null, path, Type);
        }

        public override string ToString()
        {
            return Type.ToString();
        }

        public override Evolution Evolution
        {
            get { return _effect.Evolution; }
        }
    }
}
