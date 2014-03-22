function LOG(str)
{
    console.log(str);
}

function float2(x, y)
{
    this.left = x;
    this.top = y;

    this.Add = function(other)
    {
        this.left += other.left;
        this.top += other.top;
    };

    this.Sub = function(other)
    {
        this.left -= other.left;
        this.top -= other.top;        
    }

    this.Mul = function(other)
    {
        this.left *= other;
        this.top *= other;
    }

    this.LengthSq = function()
    {
        return this.left * this.left + this.top * this.top;
    };

    this.Length = function()
    {
        return Math.sqrt(this.LengthSq());
    }

    this.DistanceSq = function(other)
    {
        var diff = float2(this.m_X, this.m_Y);
        diff.Sub(other);
        return diff.LengthSq();
    }

    this.Distance = function(other)
    {
        return Math.sqrt(this.DistanceSq(other));
    }
}