using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;
using System.Windows.Input;
using System.Windows.Media;

namespace HappyFxEditorContextLib.TimeLine
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


        public TimeLineTrackContext(TimeLineContext timeLine)
        {
            TimeLine = timeLine;
            Components = new UndoableCollection<ComponentContext>(timeLine.Effect);
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
            var component = new ComponentContext(TimeLine.SelectedComponentTool.Type)
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
                              ? xPos - (SelectedComponent.X + SelectedComponent.Width)
                              : xPos - (SelectedComponent.X);
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
                        MoveLeft(_moveXPos - MIN_COMPONENT_WIDTH / 2 - SelectedComponent.X);
                        _leftAligned = true;
                    }
                    MoveRight(xPos - (SelectedComponent.X + SelectedComponent.Width));
                }
                else if (dir < 0)
                {
                    if (_leftAligned == true)
                    {
                        MoveRight(_moveXPos + MIN_COMPONENT_WIDTH/2 - (SelectedComponent.X + SelectedComponent.Width));
                        _leftAligned = false;
                    }
                    MoveLeft(xPos - SelectedComponent.X);                
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
            int prevX = SelectedComponent.X + SelectedComponent.Width;
            int newX = prevX + move;

            if (newX < prevX)
            {
                newX = Math.Max(SelectedComponent.X + MIN_COMPONENT_WIDTH, newX);
            }
            else
            {
                foreach (var c in Components)
                {
                    if (c != SelectedComponent && newX > c.X && c.X >= prevX)
                        newX = Math.Min(newX, c.X);
                }             
            }
            SelectedComponent.Width = newX - SelectedComponent.X; 

            return newX - prevX;
        }
        private int MoveLeft(int move)
        {
            int prevX = SelectedComponent.X;
            int newX = prevX + move;

            if (newX > prevX)
            {
                newX = Math.Min(SelectedComponent.X + SelectedComponent.Width - MIN_COMPONENT_WIDTH, newX);
            }
            else
            {
                foreach (var c in Components)
                {
                    if (c != SelectedComponent && newX < c.X + c.Width && c.X + c.Width <= prevX)
                        newX = Math.Max(newX, c.X + c.Width);
                }
            }
            SelectedComponent.Width = prevX + SelectedComponent.Width - newX;
            SelectedComponent.X = newX;

            return newX - prevX;
        }

        internal void Remove(ComponentContext componentContext)
        {
            Components.Remove(componentContext);
            TimeLine.SelectedComponent = null;
        }
    }
}
