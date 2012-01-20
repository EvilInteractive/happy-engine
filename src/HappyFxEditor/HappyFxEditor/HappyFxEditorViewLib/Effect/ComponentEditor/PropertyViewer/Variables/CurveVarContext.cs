using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;
using System.Windows.Media;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    public enum CurveCornerType
    {
        Corner,
        Bezier
    }
    public class CurvePointContext : UndoableChangeSource
    {
        #region Time
        private float _time;
        public const string TimeProperty = "Time";
        public float Time
        {
            get { return _time; }
            set { Change(_time, IsTimeLocked?MathHelper.Clamp(value, 0.0f, 1.0f):
                                             MathHelper.Clamp(value, 0.001f, 0.999f), (newValue) => Swap(ref _time, newValue, TimeProperty));
            }
        }
        #endregion

        #region IsTimeLocked
        private bool _isTimeLocked;
        public const string IsTimeLockedProperty = "IsTimeLocked";
        public bool IsTimeLocked
        {
            get { return _isTimeLocked; }
            set { Change(ref _isTimeLocked, value, IsTimeLockedProperty); }
        }
        #endregion
        

        #region Variable
        private ConstantVarContext _variable;
        public const string VariableProperty = "Variable";
        public ConstantVarContext Variable
        {
            get { return _variable; }
            set { Change(ref _variable, value, VariableProperty); }
        }
        #endregion

        #region Tangent1
        private Vector _tangent1;
        public const string Tangent1Property = "Tangent1";
        public Vector Tangent1
        {
            get { return _tangent1; }
            set { Change(ref _tangent1, value, Tangent1Property); }
        }
        #endregion

        #region Tangent2
        private Vector _tangent2;
        public const string Tangent2Property = "Tangent2";
        public Vector Tangent2
        {
            get { return _tangent2; }
            set { Change(ref _tangent2, value, Tangent2Property); }
        }
        #endregion

        #region CornerType
        private CurveCornerType _cornerType;
        public const string CornerTypeProperty = "CornerType";
        public CurveCornerType CornerType
        {
            get { return _cornerType; }
            set { Change(ref _cornerType, value, CornerTypeProperty); }
        }
        #endregion        

        #region IsSelected
        private bool _isSelected;
        public const string IsSelectedProperty = "IsSelected";
        public bool IsSelected
        {
            get { return _isSelected; }
            set { Change(ref _isSelected, value, IsSelectedProperty); }
        }
        #endregion
        

        public CurvePointContext(Evolution evolution)
        {
            _evolution = evolution;
            IsTimeLocked = false;
        }

        public CurvePointContext Copy(EffectContext effect)
        {
            CurvePointContext temp = new CurvePointContext(effect.Evolution);
            temp.Time = Time;
            temp.Variable = (ConstantVarContext)Variable.Copy(effect);
            temp.IsTimeLocked = IsTimeLocked;

            return temp;
        }

        private Evolution _evolution;
        public override Evolution Evolution
        {
            get { return _evolution; }
        }

        internal void Serialize(BinaryWriter stream)
        {
            stream.Write((float)Time);
            Variable.Serialize(stream);
        }

        internal void DeSerialize(BinaryReader stream)
        {
            Time = stream.ReadSingle();
            Variable.DeSerialize(stream);
        }
    }
    public class CurveVarContext : PropertyChangeSource, IVariableContext
    {
        public UndoableCollection<CurvePointContext> Points { get; private set; }

        #region SelectedPoint
        private CurvePointContext _selectedPoint;
        public const string SelectedPointProperty = "SelectedPoint";
        public CurvePointContext SelectedPoint
        {
            get { return _selectedPoint;}
            set { Change(_selectedPoint, value, (newValue) =>
                                                    {
                                                        if (_selectedPoint != null)
                                                            _selectedPoint.IsSelected = false;
                                                        _selectedPoint = newValue;

                                                        if (_selectedPoint != null)
                                                            _selectedPoint.IsSelected = true;
                                                    }, SelectedPointProperty);
            }
        }
        #endregion

        public ICommand AddPointCommand { get; private set; }
        public ICommand RemoveSelectedCommand { get; private set; }

        public VariableType VariableType { get { return VariableType.Curve; } }
        public VariableComponentType VariableComponentType { get { return VariableComponentType.Float; } }

        private readonly ConstantVarContext _pointTemplate;

        public CurveVarContext(EffectContext effect, ConstantVarContext pointTemplate)
        {
            _effect = effect;
            _pointTemplate = pointTemplate;

            Points = new UndoableCollection<CurvePointContext>(effect);
            CurvePointContext p0 = AddPoint();
            p0.IsTimeLocked = true;
            p0.Time = 0.0f;
            p0.Variable = (ConstantVarContext)pointTemplate.Copy(effect);
            CurvePointContext p1 = AddPoint();
            p1.IsTimeLocked = true;
            p1.Time = 1.0f;
            p0.Variable = (ConstantVarContext)pointTemplate.Copy(effect);

            AddPointCommand = CommandFactory.Create(() => AddPoint());
            RemoveSelectedCommand = CommandFactory.Create(RemoveSelected,
                                                          () =>
                                                          SelectedPoint != null &&
                                                          SelectedPoint.Time > 0.0f &&
                                                          SelectedPoint.Time < 1.0f,
                                                          this, SelectedPointProperty);
        }

        public CurvePointContext AddPoint()
        {
            CurvePointContext point = new CurvePointContext(Effect.Evolution)
                                          {
                                              Time = 0.5f,
                                              Variable = (ConstantVarContext)_pointTemplate.Copy(Effect)
                                          };
            point.PropertyChanged += (s, e) =>
                                         {
                                             if (e.PropertyName == CurvePointContext.IsSelectedProperty)
                                             {
                                                 SelectedPoint = point;
                                             }
                                         };
            Points.Add(point);
            SelectedPoint = point;
            return point;
        }
        public void RemoveSelected()
        {
            Points.Remove(SelectedPoint);
            SelectedPoint = null;
        }

        public VariableType GetVarType()
        {
            return VariableType;
        }

        public IVariableContext Copy(EffectContext effect)
        {
            CurveVarContext temp = new CurveVarContext(effect, _pointTemplate);
            temp.Points = new UndoableCollection<CurvePointContext>(effect);
            foreach (var p in Points)
            {
                temp.Points.Add(p.Copy(effect));
            }
            return temp;
        }

        public void Serialize(BinaryWriter stream)
        {
            stream.Write((UInt32)Points.Count);
            foreach (var p in Points)
            {
                p.Serialize(stream);
            }
        }

        public void DeSerialize(BinaryReader stream)
        {
            uint points = stream.ReadUInt32();
            Points = new UndoableCollection<CurvePointContext>(Effect);
            for (int i = 0; i < points; ++i)
            {
                CurvePointContext p = new CurvePointContext(Effect.Evolution);
                p.Variable = (ConstantVarContext)_pointTemplate.Copy(Effect);
                p.DeSerialize(stream);
                Points.Add(p);
            }
        }

        private EffectContext _effect;
        public EffectContext Effect
        {
            get { return _effect; }
        }

        public override string ToString()
        {
            return "Curve";
        }
    }
}
