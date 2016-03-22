# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.31
#
# Don't modify this file, modify the SWIG interface instead.

import _Field
import new
new_instancemethod = new.instancemethod
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


class PySwigIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _Field.delete_PySwigIterator
    __del__ = lambda self : None;
    def value(*args): return _Field.PySwigIterator_value(*args)
    def incr(*args): return _Field.PySwigIterator_incr(*args)
    def decr(*args): return _Field.PySwigIterator_decr(*args)
    def distance(*args): return _Field.PySwigIterator_distance(*args)
    def equal(*args): return _Field.PySwigIterator_equal(*args)
    def copy(*args): return _Field.PySwigIterator_copy(*args)
    def next(*args): return _Field.PySwigIterator_next(*args)
    def previous(*args): return _Field.PySwigIterator_previous(*args)
    def advance(*args): return _Field.PySwigIterator_advance(*args)
    def __eq__(*args): return _Field.PySwigIterator___eq__(*args)
    def __ne__(*args): return _Field.PySwigIterator___ne__(*args)
    def __iadd__(*args): return _Field.PySwigIterator___iadd__(*args)
    def __isub__(*args): return _Field.PySwigIterator___isub__(*args)
    def __add__(*args): return _Field.PySwigIterator___add__(*args)
    def __sub__(*args): return _Field.PySwigIterator___sub__(*args)
    def __iter__(self): return self
PySwigIterator_swigregister = _Field.PySwigIterator_swigregister
PySwigIterator_swigregister(PySwigIterator)

class DVector(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def iterator(*args): return _Field.DVector_iterator(*args)
    def __iter__(self): return self.iterator()
    def __nonzero__(*args): return _Field.DVector___nonzero__(*args)
    def __len__(*args): return _Field.DVector___len__(*args)
    def pop(*args): return _Field.DVector_pop(*args)
    def __getslice__(*args): return _Field.DVector___getslice__(*args)
    def __setslice__(*args): return _Field.DVector___setslice__(*args)
    def __delslice__(*args): return _Field.DVector___delslice__(*args)
    def __delitem__(*args): return _Field.DVector___delitem__(*args)
    def __getitem__(*args): return _Field.DVector___getitem__(*args)
    def __setitem__(*args): return _Field.DVector___setitem__(*args)
    def append(*args): return _Field.DVector_append(*args)
    def empty(*args): return _Field.DVector_empty(*args)
    def size(*args): return _Field.DVector_size(*args)
    def clear(*args): return _Field.DVector_clear(*args)
    def swap(*args): return _Field.DVector_swap(*args)
    def get_allocator(*args): return _Field.DVector_get_allocator(*args)
    def begin(*args): return _Field.DVector_begin(*args)
    def end(*args): return _Field.DVector_end(*args)
    def rbegin(*args): return _Field.DVector_rbegin(*args)
    def rend(*args): return _Field.DVector_rend(*args)
    def pop_back(*args): return _Field.DVector_pop_back(*args)
    def erase(*args): return _Field.DVector_erase(*args)
    def __init__(self, *args): 
        this = _Field.new_DVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args): return _Field.DVector_push_back(*args)
    def front(*args): return _Field.DVector_front(*args)
    def back(*args): return _Field.DVector_back(*args)
    def assign(*args): return _Field.DVector_assign(*args)
    def resize(*args): return _Field.DVector_resize(*args)
    def insert(*args): return _Field.DVector_insert(*args)
    def reserve(*args): return _Field.DVector_reserve(*args)
    def capacity(*args): return _Field.DVector_capacity(*args)
    __swig_destroy__ = _Field.delete_DVector
    __del__ = lambda self : None;
DVector_swigregister = _Field.DVector_swigregister
DVector_swigregister(DVector)

class ULongVector(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def iterator(*args): return _Field.ULongVector_iterator(*args)
    def __iter__(self): return self.iterator()
    def __nonzero__(*args): return _Field.ULongVector___nonzero__(*args)
    def __len__(*args): return _Field.ULongVector___len__(*args)
    def pop(*args): return _Field.ULongVector_pop(*args)
    def __getslice__(*args): return _Field.ULongVector___getslice__(*args)
    def __setslice__(*args): return _Field.ULongVector___setslice__(*args)
    def __delslice__(*args): return _Field.ULongVector___delslice__(*args)
    def __delitem__(*args): return _Field.ULongVector___delitem__(*args)
    def __getitem__(*args): return _Field.ULongVector___getitem__(*args)
    def __setitem__(*args): return _Field.ULongVector___setitem__(*args)
    def append(*args): return _Field.ULongVector_append(*args)
    def empty(*args): return _Field.ULongVector_empty(*args)
    def size(*args): return _Field.ULongVector_size(*args)
    def clear(*args): return _Field.ULongVector_clear(*args)
    def swap(*args): return _Field.ULongVector_swap(*args)
    def get_allocator(*args): return _Field.ULongVector_get_allocator(*args)
    def begin(*args): return _Field.ULongVector_begin(*args)
    def end(*args): return _Field.ULongVector_end(*args)
    def rbegin(*args): return _Field.ULongVector_rbegin(*args)
    def rend(*args): return _Field.ULongVector_rend(*args)
    def pop_back(*args): return _Field.ULongVector_pop_back(*args)
    def erase(*args): return _Field.ULongVector_erase(*args)
    def __init__(self, *args): 
        this = _Field.new_ULongVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args): return _Field.ULongVector_push_back(*args)
    def front(*args): return _Field.ULongVector_front(*args)
    def back(*args): return _Field.ULongVector_back(*args)
    def assign(*args): return _Field.ULongVector_assign(*args)
    def resize(*args): return _Field.ULongVector_resize(*args)
    def insert(*args): return _Field.ULongVector_insert(*args)
    def reserve(*args): return _Field.ULongVector_reserve(*args)
    def capacity(*args): return _Field.ULongVector_capacity(*args)
    __swig_destroy__ = _Field.delete_ULongVector
    __del__ = lambda self : None;
ULongVector_swigregister = _Field.ULongVector_swigregister
ULongVector_swigregister(ULongVector)

class Mesh(object):
    """Proxy of C++ Mesh class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self, double length, double area, unsigned int num_zones) -> Mesh
        __init__(self, DVector zone_lengths, double area=1.0) -> Mesh
        __init__(self, DVector zone_lengths) -> Mesh
        """
        this = _Field.new_Mesh(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _Field.delete_Mesh
    __del__ = lambda self : None;
    def length(*args):
        """length(self) -> double"""
        return _Field.Mesh_length(*args)

    def numZones(*args):
        """numZones(self) -> unsigned int"""
        return _Field.Mesh_numZones(*args)

    def numNodes(*args):
        """numNodes(self) -> unsigned int"""
        return _Field.Mesh_numNodes(*args)

    def numCorners(*args):
        """numCorners(self) -> unsigned int"""
        return _Field.Mesh_numCorners(*args)

Mesh_swigregister = _Field.Mesh_swigregister
Mesh_swigregister(Mesh)

class Material(object):
    """Proxy of C++ Material class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> Material
        __init__(self, double xS, double xF, double xG, double nu) -> Material
        __init__(self, Material M) -> Material
        """
        this = _Field.new_Material(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _Field.delete_Material
    __del__ = lambda self : None;
    def xS(*args):
        """xS(self) -> double"""
        return _Field.Material_xS(*args)

    def xF(*args):
        """xF(self) -> double"""
        return _Field.Material_xF(*args)

    def xG(*args):
        """xG(self) -> double"""
        return _Field.Material_xG(*args)

    def xA(*args):
        """xA(self) -> double"""
        return _Field.Material_xA(*args)

    def xT(*args):
        """xT(self) -> double"""
        return _Field.Material_xT(*args)

    def nu(*args):
        """nu(self) -> double"""
        return _Field.Material_nu(*args)

    def stringRep(*args):
        """stringRep(self) -> string"""
        return _Field.Material_stringRep(*args)

    def __str__(*args):
        """__str__(self) -> string"""
        return _Field.Material___str__(*args)

Material_swigregister = _Field.Material_swigregister
Material_swigregister(Material)

class vectormats(object):
    """Proxy of C++ vectormats class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def iterator(*args):
        """iterator(self, PyObject PYTHON_SELF) -> PySwigIterator"""
        return _Field.vectormats_iterator(*args)

    def __iter__(self): return self.iterator()
    def __nonzero__(*args):
        """__nonzero__(self) -> bool"""
        return _Field.vectormats___nonzero__(*args)

    def __len__(*args):
        """__len__(self) -> size_type"""
        return _Field.vectormats___len__(*args)

    def pop(*args):
        """pop(self) -> value_type"""
        return _Field.vectormats_pop(*args)

    def __getslice__(*args):
        """__getslice__(self, difference_type i, difference_type j) -> vectormats"""
        return _Field.vectormats___getslice__(*args)

    def __setslice__(*args):
        """__setslice__(self, difference_type i, difference_type j, vectormats v)"""
        return _Field.vectormats___setslice__(*args)

    def __delslice__(*args):
        """__delslice__(self, difference_type i, difference_type j)"""
        return _Field.vectormats___delslice__(*args)

    def __delitem__(*args):
        """__delitem__(self, difference_type i)"""
        return _Field.vectormats___delitem__(*args)

    def __getitem__(*args):
        """__getitem__(self, difference_type i) -> value_type"""
        return _Field.vectormats___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, difference_type i, value_type x)"""
        return _Field.vectormats___setitem__(*args)

    def append(*args):
        """append(self, value_type x)"""
        return _Field.vectormats_append(*args)

    def empty(*args):
        """empty(self) -> bool"""
        return _Field.vectormats_empty(*args)

    def size(*args):
        """size(self) -> size_type"""
        return _Field.vectormats_size(*args)

    def clear(*args):
        """clear(self)"""
        return _Field.vectormats_clear(*args)

    def swap(*args):
        """swap(self, vectormats v)"""
        return _Field.vectormats_swap(*args)

    def get_allocator(*args):
        """get_allocator(self) -> allocator_type"""
        return _Field.vectormats_get_allocator(*args)

    def begin(*args):
        """
        begin(self) -> iterator
        begin(self) -> const_iterator
        """
        return _Field.vectormats_begin(*args)

    def end(*args):
        """
        end(self) -> iterator
        end(self) -> const_iterator
        """
        return _Field.vectormats_end(*args)

    def rbegin(*args):
        """
        rbegin(self) -> reverse_iterator
        rbegin(self) -> const_reverse_iterator
        """
        return _Field.vectormats_rbegin(*args)

    def rend(*args):
        """
        rend(self) -> reverse_iterator
        rend(self) -> const_reverse_iterator
        """
        return _Field.vectormats_rend(*args)

    def pop_back(*args):
        """pop_back(self)"""
        return _Field.vectormats_pop_back(*args)

    def erase(*args):
        """
        erase(self, iterator pos) -> iterator
        erase(self, iterator first, iterator last) -> iterator
        """
        return _Field.vectormats_erase(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> vectormats
        __init__(self, vectormats ?) -> vectormats
        __init__(self, size_type size) -> vectormats
        __init__(self, size_type size, value_type value) -> vectormats
        """
        this = _Field.new_vectormats(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args):
        """push_back(self, value_type x)"""
        return _Field.vectormats_push_back(*args)

    def front(*args):
        """front(self) -> value_type"""
        return _Field.vectormats_front(*args)

    def back(*args):
        """back(self) -> value_type"""
        return _Field.vectormats_back(*args)

    def assign(*args):
        """assign(self, size_type n, value_type x)"""
        return _Field.vectormats_assign(*args)

    def resize(*args):
        """
        resize(self, size_type new_size)
        resize(self, size_type new_size, value_type x)
        """
        return _Field.vectormats_resize(*args)

    def insert(*args):
        """
        insert(self, iterator pos, value_type x) -> iterator
        insert(self, iterator pos, size_type n, value_type x)
        """
        return _Field.vectormats_insert(*args)

    def reserve(*args):
        """reserve(self, size_type n)"""
        return _Field.vectormats_reserve(*args)

    def capacity(*args):
        """capacity(self) -> size_type"""
        return _Field.vectormats_capacity(*args)

    __swig_destroy__ = _Field.delete_vectormats
    __del__ = lambda self : None;
vectormats_swigregister = _Field.vectormats_swigregister
vectormats_swigregister(vectormats)

class FieldZoneDouble(object):
    """Proxy of C++ FieldZoneDouble class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self, Mesh mesh) -> FieldZoneDouble
        __init__(self, Mesh mesh, DVector data) -> FieldZoneDouble
        __init__(self, FieldZoneDouble field_in) -> FieldZoneDouble
        """
        this = _Field.new_FieldZoneDouble(*args)
        try: self.this.append(this)
        except: self.this = this
    def getMesh(*args):
        """getMesh(self) -> Mesh"""
        return _Field.FieldZoneDouble_getMesh(*args)

    def __getitem__(*args):
        """__getitem__(self, Zone elem) -> double"""
        return _Field.FieldZoneDouble___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, Zone elem, double value)"""
        return _Field.FieldZoneDouble___setitem__(*args)

    def __len__(*args):
        """__len__(self) -> unsigned int"""
        return _Field.FieldZoneDouble___len__(*args)

    __swig_destroy__ = _Field.delete_FieldZoneDouble
    __del__ = lambda self : None;
FieldZoneDouble_swigregister = _Field.FieldZoneDouble_swigregister
FieldZoneDouble_swigregister(FieldZoneDouble)

class FieldNodeDouble(object):
    """Proxy of C++ FieldNodeDouble class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self, Mesh mesh) -> FieldNodeDouble
        __init__(self, Mesh mesh, DVector data) -> FieldNodeDouble
        __init__(self, FieldNodeDouble field_in) -> FieldNodeDouble
        """
        this = _Field.new_FieldNodeDouble(*args)
        try: self.this.append(this)
        except: self.this = this
    def getMesh(*args):
        """getMesh(self) -> Mesh"""
        return _Field.FieldNodeDouble_getMesh(*args)

    def __getitem__(*args):
        """__getitem__(self, Node elem) -> double"""
        return _Field.FieldNodeDouble___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, Node elem, double value)"""
        return _Field.FieldNodeDouble___setitem__(*args)

    def __len__(*args):
        """__len__(self) -> unsigned int"""
        return _Field.FieldNodeDouble___len__(*args)

    __swig_destroy__ = _Field.delete_FieldNodeDouble
    __del__ = lambda self : None;
FieldNodeDouble_swigregister = _Field.FieldNodeDouble_swigregister
FieldNodeDouble_swigregister(FieldNodeDouble)

class FieldZoneMat(object):
    """Proxy of C++ FieldZoneMat class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self, Mesh mesh) -> FieldZoneMat
        __init__(self, Mesh mesh, vectormats data) -> FieldZoneMat
        __init__(self, FieldZoneMat field_in) -> FieldZoneMat
        """
        this = _Field.new_FieldZoneMat(*args)
        try: self.this.append(this)
        except: self.this = this
    def getMesh(*args):
        """getMesh(self) -> Mesh"""
        return _Field.FieldZoneMat_getMesh(*args)

    def __getitem__(*args):
        """__getitem__(self, Zone elem) -> Material"""
        return _Field.FieldZoneMat___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, Zone elem, Material value)"""
        return _Field.FieldZoneMat___setitem__(*args)

    def __len__(*args):
        """__len__(self) -> unsigned int"""
        return _Field.FieldZoneMat___len__(*args)

    __swig_destroy__ = _Field.delete_FieldZoneMat
    __del__ = lambda self : None;
FieldZoneMat_swigregister = _Field.FieldZoneMat_swigregister
FieldZoneMat_swigregister(FieldZoneMat)


