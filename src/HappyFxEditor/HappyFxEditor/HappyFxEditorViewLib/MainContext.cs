using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using DaeMvvmFramework;
using HappyFxEditorBaseLib.Net;
using HappyFxEditorContextLib.Browsers;
using HappyFxEditorContextLib.Effect;

namespace HappyFxEditorContextLib
{
    public class MainContext : PropertyChangeSource
    {
        ///////////////////////////////////////////////////////////////////
        //     Fields
        ///////////////////////////////////////////////////////////////////   
        public ICommand NewCommand { get; private set; }
        public ICommand OpenCommand { get; private set; }
        public ICommand SaveCommand { get; private set; }
        public ICommand SaveAsCommand { get; private set; }
        public ICommand SaveAllCommand { get; private set; }
        public ICommand CloseCommand { get; private set; }
        public ICommand CloseAllCommand { get; private set; }
        public ICommand ExitCommand { get; private set; }

        ///////////////////////////////////////////////////////////////////
        //     Ctor
        ///////////////////////////////////////////////////////////////////   
        public MainContext()
        {
            NewCommand = CommandFactory.Create(NewEffect);
            OpenCommand = CommandFactory.Create(OpenEffect);
            SaveCommand = CommandFactory.Create(SaveEffect, CanSave, this, CurrentEffectProperty);
            SaveAsCommand = CommandFactory.Create(SaveEffectAs, CanSaveAs, this, CurrentEffectProperty);
            SaveAllCommand = CommandFactory.Create(SaveAllEffects, CanSave, this, CurrentEffectProperty);
            CloseCommand = CommandFactory.Create(Close, CanClose, this, CurrentEffectProperty);
            CloseAllCommand = CommandFactory.Create(CloseAll, CanClose, this, CurrentEffectProperty);
            ExitCommand = CommandFactory.Create(Exit);

            OpenEffects = new ObservableCollection<EffectContext>();
            AssetManager = new AssetManagerContext();
            AssetManager.Refresh();
        }

        
        ///////////////////////////////////////////////////////////////////
        //     Properties
        ///////////////////////////////////////////////////////////////////    
        public ObservableCollection<EffectContext> OpenEffects { get; private set; }
        private EffectContext _currentEffect;
        public static readonly string CurrentEffectProperty = "CurrentEffect";
        public EffectContext CurrentEffect
        {
            get { return _currentEffect; }
            set { Change(ref _currentEffect, value, CurrentEffectProperty); }
        }

        public AssetManagerContext AssetManager { get; private set; }

        ///////////////////////////////////////////////////////////////////
        //     Methods
        ///////////////////////////////////////////////////////////////////
        
        public void Exit()
        {
            if (OpenEffects.Count != 0)
                CloseAll();
            HeConnectionManager.Instance.Dispose();
            if (OpenEffects.Count == 0)
                Application.Current.Shutdown();
        }

        public void NewEffect()
        {
            OpenEffects.Add(new EffectContext(this));
            CurrentEffect = OpenEffects[OpenEffects.Count - 1];
        }

        public void Close()
        {
            Debug.Assert(CurrentEffect != null, "Shouldn't be able to close when there is no effect");
            if (CurrentEffect.Modified)
            {
                if (MessageBox.Show("Do you like to save '" + CurrentEffect.Name + "' before closing?",
                    "Save?", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.Yes)
                {
                    SaveEffect();
                }
            }
            Close(CurrentEffect);
        }
        public void Close(EffectContext effect)
        {
            OpenEffects.Remove(effect);
        }
        public void CloseAll()
        {
            Debug.Assert(OpenEffects.Count != 0, "Shouldn't be able to close all when there is no effect");
            foreach (var effectContext in OpenEffects.ToArray())
            {
                if (effectContext.Modified)
                {
                    MessageBoxResult result =
                        MessageBox.Show("Do you like to save '" + effectContext.Name + "' before closing?",
                                        "Save?", MessageBoxButton.YesNoCancel, MessageBoxImage.Warning);
                    if (result == MessageBoxResult.Yes)
                    {
                        SaveEffect();
                    }
                    else if (result == MessageBoxResult.Cancel)
                    {
                        break;
                    }
                }
                Close(effectContext);
            }
        }
        public bool CanClose()
        {
            return CurrentEffect != null;
        }

        public void OpenEffect()
        {
            string path = EffectSaveLoadDialogProvider.GetOpenPath(null);
            if (path != null)
            {
                OpenEffects.Add(new EffectContext(this, path));
            }
        }

        public void SaveEffect()
        {
            Debug.Assert(CurrentEffect != null, "Shouldn't be able to save when there is no effect");
            SaveEffect(CurrentEffect);
        }
        public bool SaveEffect(EffectContext effect)
        {
            if (string.IsNullOrEmpty(effect.File))
            {
                string path = EffectSaveLoadDialogProvider.GetSavePath(null);
                if (path != null)
                {
                    effect.Save(path);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                effect.Save();
                return true;
            }
        }

        public bool CanSave()
        {
            return CurrentEffect != null && CurrentEffect.Modified;
        }

        public void SaveEffectAs()
        {
            Debug.Assert(CurrentEffect != null, "Shouldn't be able to save as when there is no effect");
            string path = EffectSaveLoadDialogProvider.GetSavePath(CurrentEffect.File);
            if (path != null)
                CurrentEffect.Save(path);       
        }
        public bool CanSaveAs()
        {
            return CurrentEffect != null;
        }

        public void SaveAllEffects()
        {
            Debug.Assert(OpenEffects.Count != 0, "Shouldn't be able to save all when there are no effect");
            foreach (var effectContext in OpenEffects)
            {
                if (SaveEffect(effectContext) == false)
                    break;
            }
        }

    }
}
