using System;
using System.IO;
using System.Linq;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;
using System.Windows.Input;
using System.Windows.Media;

namespace HappyFxEditorContextLib.Effect.TimeLine
{
    public class TimeLineTrackContext: PropertyChangeSource
    {
        public TimeLineContext TimeLine { get; private set; }
        public UndoableCollection<ComponentContext> Components { get; private set; }

        private ComponentContext SelectedComponent
        {
            get { return TimeLine.SelectedComponent; }
        }

        private bool _creating = false;
        private int _moving = 0;
        private int _moveXPos = 0;
        private bool _leftAligned = true;
        private bool _createMoveModifier = false;

        private const int MIN_COMPONENT_WIDTH = 16;

        public ICommand PasteComponentCommand { get; private set; }
        public int ContextClickPos { get; set; }

        public TimeLineTrackContext(TimeLineContext timeLine)
        {
            TimeLine = timeLine;
            Components = new UndoableCollection<ComponentContext>(timeLine.Effect);

            PasteComponentCommand = CommandFactory.Create(() => TryAddComponent(((ComponentContext)TimeLine.ClipBoard).Copy()),
                                                          () =>
                                                          TimeLine.ClipBoard != null &&
                                                          TimeLine.ClipBoard.GetType() == typeof(ComponentContext),
                                                          TimeLine, TimeLineContext.ClipBoardProperty);  
        }

        private bool _isSelected = false;
        public bool IsSelected
        {
            get { return _isSelected; }
            set { Change(ref _isSelected, value, "IsSelected", "BorderSize", "BorderColor"); }
        }
        public double BorderSize 
        {
            get { return IsSelected ? 2.0 : 1.0; }
        }
        public Brush BorderColor
        {
            get { return IsSelected ? Brushes.White : Brushes.Black; }
        }

        public void OnMouseDown(int xPos)
        {
            if (TimeLine.SelectedComponentTool == null || _creating == true)
                return;
            foreach (var c in Components)
            {
                if (xPos > c.X && xPos < c.X + c.Width) //clicked on other component
                {
                    TimeLine.SelectedComponent = c;
                    _moving = 1;
                    _moveXPos = xPos;
                    return;
                }
            }

            if (TimeLine.SelectedComponentTool.Type == TimeLineTrackComponentType.MAX_TYPES)
                return;

            if (Components.Any(c => xPos - MIN_COMPONENT_WIDTH/2 < c.X + c.Width && xPos + MIN_COMPONENT_WIDTH/2 > c.X))
                return;

            _creating = true;
            var component = new ComponentContext(this, TimeLine.SelectedComponentTool.Type)
                {
                    X = xPos - MIN_COMPONENT_WIDTH / 2,
                    Width = MIN_COMPONENT_WIDTH,
                    Height = 24
                };
            _moveXPos = xPos;
            Components.Add(component);
            TimeLine.SelectedComponent = component;
        }
        
        public void OnMouseUp(int xPos)
        {
            OnMouseMove(xPos);
            _moving = 0;
            _creating = false;
        }
        public void OnMouseMove(int xPos)
        {
            if (_moving == 1)
            {
                if (Math.Abs(xPos - _moveXPos) > 4)
                    _moving = 2;
            }
            if (_moving == 2)
            {
                int dir = xPos - _moveXPos;
                if (dir > 0)
                {
                    int realMove = MoveRight(dir);
                    MoveLeft(realMove);
                    _moveXPos += realMove;
                }
                else if (dir < 0)
                {
                    int realMove = MoveLeft(dir);
                    MoveRight(realMove);
                    _moveXPos += realMove;
                }         
            }


            if (_creating == false)
                return;

            // Location
            if (_createMoveModifier)
            {
                int dir = _leftAligned
                              ? xPos - (int)(SelectedComponent.X + SelectedComponent.Width)
                              : xPos - (int)(SelectedComponent.X);
                if (dir > 0)
                {
                    int realMove = MoveRight(dir);
                    MoveLeft(realMove);
                    _moveXPos += realMove;
                }
                else if (dir < 0)
                {
                    int realMove = MoveLeft(dir);
                    MoveRight(realMove);
                    _moveXPos += realMove;
                }
            }
            // Size
            else
            {   
                int dir = xPos - _moveXPos;
                if (dir > 0)
                {
                    if (_leftAligned == false)
                    {
                        MoveLeft(_moveXPos - MIN_COMPONENT_WIDTH / 2 - (int)SelectedComponent.X);
                        _leftAligned = true;
                    }
                    MoveRight(xPos - (int)(SelectedComponent.X + SelectedComponent.Width));
                }
                else if (dir < 0)
                {
                    if (_leftAligned == true)
                    {
                        MoveRight(_moveXPos + MIN_COMPONENT_WIDTH/2 - (int)(SelectedComponent.X + SelectedComponent.Width));
                        _leftAligned = false;
                    }
                    MoveLeft(xPos - (int)SelectedComponent.X);                
                }
            }

        }

        public void OnCreateMoveModifierPressed()
        {
            _createMoveModifier = true;
        }
        public void OnCreateMoveModifierReleased(int xPos)
        {
            _createMoveModifier = false;
            OnMouseMove(xPos);
        }

        private int MoveRight(int move)
        {
            int prevX = (int)(SelectedComponent.X + SelectedComponent.Width);
            int newX = prevX + move;

            if (newX < prevX)
            {
                newX = Math.Max((int)SelectedComponent.X + MIN_COMPONENT_WIDTH, newX);
            }
            else
            {
                foreach (var c in Components)
                {
                    if (c != SelectedComponent && newX > c.X && c.X >= prevX)
                        newX = Math.Min(newX, (int)c.X);
                }             
            }
            newX = Math.Min(newX, (int) (TimeLine.Duration*TimeLine.TimeScale));

            SelectedComponent.Width = newX - SelectedComponent.X; 

            return newX - prevX;
        }
        private int MoveLeft(int move)
        {
            int prevX = (int)SelectedComponent.X;
            int newX = prevX + move;

            if (newX > prevX)
            {
                newX = Math.Min((int)(SelectedComponent.X + SelectedComponent.Width) - MIN_COMPONENT_WIDTH, newX);
            }
            else
            {
                foreach (var c in Components)
                {
                    if (c != SelectedComponent && newX < c.X + c.Width && c.X + c.Width <= prevX)
                        newX = Math.Max(newX, (int)(c.X + c.Width));
                }
            }
            newX = Math.Max(0, newX);

            SelectedComponent.Width = prevX + SelectedComponent.Width - newX;
            SelectedComponent.X = newX;

            return newX - prevX;
        }

        private bool TryAddComponent(ComponentContext component)
        {
            int left = ContextClickPos - (int)(component.Width / 2);
            int right = ContextClickPos + (int)(component.Width / 2);

            int realLeft = left;
            int realRight = right;

            foreach (var c in Components)
            {
                if (c.X + c.Width < ContextClickPos)
                    realLeft = Math.Max(realLeft, (int)(c.X + c.Width));
                if (c.X > ContextClickPos)
                    realRight = Math.Min(realRight, (int)c.X);
            }

            if (left != realLeft && right != realRight)
                return false;

            if (left != realLeft)
            { // try expand right
                right += realLeft - left;
                realRight = right;
                foreach (var c in Components)
                {
                    if (c.X > ContextClickPos)
                        realRight = Math.Min(realRight, (int)c.X);
                }
                if (right != realRight)
                    return false;
            }
            else
            { // try expand left
                left += realRight - right;
                realLeft = left;
                foreach (var c in Components)
                {
                    if (c.X + c.Width < ContextClickPos)
                        realLeft = Math.Max(realLeft, (int)(c.X + c.Width));
                }
                if (left != realLeft)
                    return false;
            }

            component.X = realLeft;
            component.Width = realRight - realLeft;
            Components.Add(component);

            return true;
        }

        internal void Remove(ComponentContext componentContext)
        {
            Components.Remove(componentContext);
            TimeLine.SelectedComponent = null;
        }

        internal void Serialize(BinaryWriter stream)
        {
            stream.Write((UInt32)Components.Count);
            foreach (var comp in Components)
            {
                comp.Serialize(stream);
            }
        }

        public static TimeLineTrackContext DeSerialize(BinaryReader stream, TimeLineContext timeLineContext)
        {
            TimeLineTrackContext track = new TimeLineTrackContext(timeLineContext);

            uint components = stream.ReadUInt32();
            for (int i = 0; i < components; i++)
            {
                track.Components.Add(ComponentContext.DeSerialize(stream, track));
            }

            return track;
        }
    }
}
