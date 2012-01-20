using System;
using System.IO;
using System.Windows;
using System.Windows.Input;
using DaeMvvmFramework;
using HappyFxEditorBaseLib.Net;
using HappyFxEditorContextLib.Effect.ComponentEditor;
using HappyFxEditorContextLib.Effect.TimeLine;

namespace HappyFxEditorContextLib.Effect
{
    public sealed class EffectContext : UndoableContext
    {
        private string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
        public string DisplayName
        {
            get { return Modified ? _name + "*" : _name; }
        }

        public string File { get; set; }

        private bool _modified = false;
        public bool Modified 
        {
            get { return _modified; }
            set { Change(ref _modified, value, "Modified", "DisplayName"); }
        }

        public ICommand CloseCommand { get; private set; }
        public ICommand DeleteCommand { get; private set; }
        public ICommand PlayCommand { get; private set; }

        public MainContext MainContext { get; private set; }
        public TimeLineContext TimeLine { get; private set; }
        public ComponentEditorContext ComponentEditor { get; private set; }

        private string _fxTestPath;

        public EffectContext(MainContext mainContext, string path)
            : base(new Evolution(100))
        {
            File = path;
            Name = Path.GetFileNameWithoutExtension(path);
            MainContext = mainContext;
            Modified = false;
            CloseCommand = CommandFactory.Create(() => MainContext.Close(this));

            ComponentEditor = new ComponentEditorContext(this);
            if (string.IsNullOrEmpty(path) == false)
            {
                BinaryReader stream = null;
                try
                {
                    stream = new BinaryReader(new FileStream(path, FileMode.Open, FileAccess.Read));
                    DeSerialize(stream);
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                }
                finally
                {
                    if (stream != null)
                        stream.Close();
                }
            }
            else
            {
                TimeLine = new TimeLineContext(this);               
            }
            ComponentEditor.Init();
            Evolution.Changed += (s, e) => { Modified = true; };

            DeleteCommand = CommandFactory.Create(
            () =>
            {
                if (ComponentEditor.DeleteBehaviourCommand.CanExecute(null))
                    ComponentEditor.DeleteBehaviourCommand.Execute(null);
                else if (TimeLine.DeleteComponentCommand.CanExecute(null))
                    TimeLine.DeleteComponentCommand.Execute(null);
                else if (TimeLine.DeleteTrackCommand.CanExecute(null))
                    TimeLine.DeleteTrackCommand.Execute(null);
            },
            () => TimeLine.DeleteComponentCommand.CanExecute(null) ||
                TimeLine.DeleteTrackCommand.CanExecute(null) ||
                ComponentEditor.DeleteBehaviourCommand.CanExecute(null));

            PlayCommand = CommandFactory.Create(Play);

            HeConnectionManager.Instance.PacketReceived += (h, p) =>
                                                               {
                                                                   if (h == FxInHeader.SetEffectPath)
                                                                   {
                                                                       _fxTestPath = p.ReadString();
                                                                   }
                                                               };
            HeConnectionManager.Instance.SendPacket(new Packet(FxOutHeader.GetEffectPath));
        }
        public EffectContext(MainContext mainContext)
            : this(mainContext, "")
        {
            Name = "NewEffect";
        }

        public void Save()
        {
            Save(File);
        }
        public void Save(string path)
        {
            File = path;
            Name = Path.GetFileNameWithoutExtension(path);
            Modified = false;
            BinaryWriter stream = new BinaryWriter(new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write));
            TimeLine.Serialize(stream);
            stream.Close();
        }

        public void Play()
        {
            BinaryWriter stream = new BinaryWriter(new FileStream(_fxTestPath + "__fxeditor__.hfx", FileMode.OpenOrCreate, FileAccess.Write));
            TimeLine.Serialize(stream);
            stream.Close();
            
            HeConnectionManager.Instance.SendPacket(new Packet(FxOutHeader.Play));
        }

        private void DeSerialize(BinaryReader stream)
        {
            TimeLine = TimeLineContext.DeSerialize(stream, this);
            Evolution.Clear();
        }
    }
}
