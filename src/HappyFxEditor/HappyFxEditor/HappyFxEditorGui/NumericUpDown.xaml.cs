using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using HappyFxEditorBaseLib;

namespace HappyFxEditorApp
{
    /// <summary>
    /// Interaction logic for NumericUpDown.xaml
    /// </summary>
    public partial class NumericUpDown : UserControl, INotifyPropertyChanged
    {
        #region Value
        public float Value
        {
            get { return (float)GetValue(ValueProperty); }
            set 
            { 
                SetValue(ValueProperty, value);
                DisplayValue = value.ToString("F");
            }
        }
        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register("Value", typeof(float), typeof(NumericUpDown), new UIPropertyMetadata(0.0f));
        #endregion

        private string _displayValue;
        public string DisplayValue
        {
            get { return _displayValue; }
            private set 
            {
                //if (_displayValue != value) //Textbox could've changed value
                //{
                    _displayValue = value;
                    OnPropertyChanged("DisplayValue");
                //}
            }
        }

        #region Increment
        public float Increment
        {
            get { return (float)GetValue(IncrementProperty); }
            set { SetValue(IncrementProperty, value); }
        }
        public static readonly DependencyProperty IncrementProperty =
            DependencyProperty.Register("Increment", typeof(float), typeof(NumericUpDown), new UIPropertyMetadata(1.0f));
        #endregion

        #region Minimum
        public float Minimum
        {
            get { return (float)GetValue(MinimumProperty); }
            set { SetValue(MinimumProperty, value); }
        }
        public static readonly DependencyProperty MinimumProperty =
            DependencyProperty.Register("Minimum", typeof(float), typeof(NumericUpDown), new UIPropertyMetadata(0.0f));
        #endregion

        #region Maximum
        public float Maximum
        {
            get { return (float)GetValue(MaximumProperty); }
            set { SetValue(MaximumProperty, value); }
        }
        public static readonly DependencyProperty MaximumProperty =
            DependencyProperty.Register("Maximum", typeof(float), typeof(NumericUpDown), new UIPropertyMetadata(100.0f));
        #endregion

        public NumericUpDown()
        {
            InitializeComponent();
            DisplayValue = Value.ToString("F");
        }

        private void IncrementClick(object sender, RoutedEventArgs e)
        {
            Value = MathHelper.Clamp(Value + Increment, Minimum, Maximum);
        }

        private void DecrementClick(object sender, RoutedEventArgs e)
        {
            Value = MathHelper.Clamp(Value - Increment, Minimum, Maximum);
        }

        private void TextChanged(object sender, EventArgs e)
        {
            float value;
            if (float.TryParse(((TextBox)sender).Text, out value))
            {
                Value = MathHelper.Clamp(value, Minimum, Maximum);
            }
            else
            {
                Value = Minimum;
            }
        }
        private void TextChangedFocusLost(object sender, KeyboardFocusChangedEventArgs e)
        {
            TextChanged(sender, e);
        }
        private void TextChangedKey(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                TextChanged(sender, e);
        }

        void OnPropertyChanged(string prop)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
                handler(this, new PropertyChangedEventArgs(prop));
        }
        public event PropertyChangedEventHandler PropertyChanged;


    }
}
