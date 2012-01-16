using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
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

        public CurvePointContext Copy()
        {
            CurvePointContext temp = new CurvePointContext(Evolution);
            temp.Time = Time;
            temp.Variable = (ConstantVarContext)Variable.Copy();

            return temp;
        }

        private Evolution _evolution;
        public override Evolution Evolution
        {
            get { return _evolution; }
        }
    }
    public struct Range
    {
        public float Min { get; set; }
        public float Max { get; set; }
    }
    public class CurveVarContext : UndoableChangeSource, IVariableContext
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
        public VariableComponentCount VariableComponentCount { get; private set; }
        public VariableComponentType VariableComponentType { get { return VariableComponentType.Float; } }

        private readonly List<Range> _ranges;

        public CurveVarContext(Evolution evolution, VariableComponentCount components, List<Range> ranges)
        {
            VariableComponentCount = components;
            _ranges = ranges;
            _evolution = evolution;

            Points = new UndoableCollection<CurvePointContext>(this);
            CurvePointContext p0 = AddPoint();
            p0.IsTimeLocked = true;
            p0.Time = 0.0f;
            p0.Variable = new ConstantVarContext(components, new FloatType(_evolution, ranges[0].Min, ranges[0].Max, 0.0f));
            for (int i = 0; i < (int)components + 1; ++i)
            {
                FloatType f = (FloatType) p0.Variable.Value[i];
                f.Min = ranges[i].Min;
                f.Max = ranges[i].Max;
                f.Value = 0.0f;
            }
            CurvePointContext p1 = AddPoint();
            p1.IsTimeLocked = true;
            p1.Time = 1.0f;
            p1.Variable = new ConstantVarContext(components, new FloatType(_evolution, ranges[0].Min, ranges[0].Max, 1.0f));
            for (int i = 0; i < (int)components + 1; ++i)
            {
                FloatType f = (FloatType) p1.Variable.Value[i];
                f.Min = ranges[i].Min;
                f.Max = ranges[i].Max;
                f.Value = 1.0f;
            }

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
            CurvePointContext point = new CurvePointContext(Evolution) { Time = 0.5f, Variable = new ConstantVarContext(GetComponentCount(), new FloatType(Evolution, _ranges[0].Min, _ranges[0].Max, 0.0f)) };
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
        public VariableComponentType GetVarComponentType()
        {
            return VariableComponentType;
        }
        public VariableComponentCount GetComponentCount()
        {
            return VariableComponentCount;
        }

        public IVariableContext Copy()
        {
            CurveVarContext temp = new CurveVarContext(Evolution, VariableComponentCount, _ranges);
            temp.Points = new UndoableCollection<CurvePointContext>(temp);
            foreach (var p in Points)
            {
                temp.Points.Add(p.Copy());
            }
            return temp;
        }

        private Evolution _evolution;
        public override Evolution Evolution
        {
            get { return _evolution; }
        }

        public override string ToString()
        {
            return "Curve " + VariableComponentCount;
        }
    }
}
