/** \mainpage The Rice Vector Library 

Authors: Anthony D. Padula, Shannon D. Scott, and William W. Symes<br>
Other contributors: Hala Dajani, Marco Enriquez, Dong Sun

<p>
RVL defines computational types realizing the principal concepts of calculus in vector spaces, in the form of C++ classes. Brief descriptions of the main classes follow. For a more complete description of each, and for a list of auxiliary classes and functions, see the <a href="http://portal.acm.org/citation.cfm?doid=1499096.1499097">TOMS paper</a> and the class documentation.
<p>
<ul>
  <li><b>Vector space:</b> mathematically, a set and a number field, together with a linear combination operation, satisfying certain axioms. The computational realization of an (infinite) set is a <i>factory</i>, that is, a object (RVL::Space) which will return a dynamically allocated data structure necessary to describe a vector, upon request. The linear combination operation is an attribute of the space. The vector spaces emulated by RVL are also equipped with an inner product, that is, are Hilbert spaces. The only coordinate-invariant choice of vector is the zero vector. Spaces must be comparable, so that one can tell whether a vector is a member or not. Thus RVL::Space has five major attributes: a protected method building vector data, and four public methods: inner, linComb, zero (applying to member vectors) and operator== (comparison of spaces). The number field associated with the space is a template parameter of RVL::Space. RVL::Space is an abstract (pure virtual) base class.
<p>
 In principle, defining a usable (concrete) subclass involves implementing all five basic methods. RVL provides an partially implemented RVL::StdSpace subclass which implements the three vector methods and encapsulates the others, for spaces whose vector data structure is <i>local</i>, that is, consists of (any number of) elemental parts which expose scalar arrays (pointers) - see <a href="../../../local/doc/html/index.html">LocalRVL</a>. The principal work involved in implementing a new space using RVL::StdSpace is the definition of the vector instance data structure (RVL::DataContainer, see below) appropriate to the space, and designing code to test equality of spaces. Since the space comparison (operator==) will be used in all of the vector function classes listed below, implementations should be as efficient as possible.
  </li>
<p>
  <li><b>Vectors:</b> vectors are members of spaces, hence meaningless in themselves. Thus RVL::Vector is a concrete (fully implemented) class whose instances own (i) a reference to the RVL::Space of which the vector is a member, and (ii) instance data in the form of an RVL::DataContainer. RVL::Vector construction uses the RVL::Space::buildDataContainer method to acquire its RVL::DataContainer member, and delegates linear algebra methods to the RVL::Space of which it is a "member". Note that RVL::Vector is a concrete, non-virtual class, and the user will not need (nor, indeed, be able) to implement subclasses - use <i>as is!</i>
  </li>
<p>
  <li><b>Linear Operators:</b> a linear operator is a function. A function has a domain and a range, and a rule for assigning members of the former to members of the latter. A linear operator on a Hilbert space has an adjoint operator. [Strictly speaking, densely defined linear operators have well-defined adjoints, but for technical reasons all linear operators constructable in RVL are densely defined.] For these reasons, the RVL::LinearOp type has four key public attributes: (i) RVL::LinearOp::getDomain() returns an RVL::Space, as does (ii) RVL::LinearOp::getRange(); (iii) RVL::LinearOp::applyOp takes input and output Vector arguments, as does (iv) RVL::LinearOp::applyAdjOp. RVL::LinearOp is an abstract base class. The public domain and range methods are pure virtual must be implemented. The apply...Op methods are implemented, in terms of protected RVL::LinearOp::apply and RVL::LinearOp::applyAdj. This device, replicated in the other function classes, allows installation of the obvious sanity test in the public (implemented) interface: calling RVL::LinearOp::applyOp with (const) input RVL::Vector and (mutable) output RVL::Vector invokes a test using the RVL::Space comparison method to assure that the input is in the domain, and the output in the range. The actual evaluation takes place in the user-defined protected functions. The important consequences of this design are that (1) the user may write the evaluation code assuming that all attributes related to domain and range spaces are correct (for example, dimensions, sample rates, array lengths, etc.), because (2) the evaluation code is only accessible via the public interface, which enforces these constraints.  

  </li>
<p>
  <li><b>Nonlinear Functions and Operators:</b> RVL uses "Functional" to describe scalar-valued functions, and "Operator" to describe vector-valued functions. Being functions, both RVL::Functional and RVL::Operator have a getDomain() method, and RVL::Operator has a getRange() method - the range of an RVL::Functional being the scalar field passed via template parameter. Those are the only public methods. Computation of value, derivative, adjoint derivative, etc. must be supplied to create a usable subclass, by implementation of protected methods declared as pure virtual in the base classes.
<p>
 Thus applications do not access the computational methods directly. Instead, access to these computations proceeds through an <i>evaluation</i> class. RVL::FunctionalEvaluation and RVL::OperatorEvaluation combine a function and a vector, and return the value of the function at that vector, the value of its derivative (a linear operator), and (optionally) the value of the second derivative (as a bilinear operator), each these encapsulated as an appropriate RVL object. While RVL::Functional and RVL::Operator are abstract base classes, and the various protecte apply... methods must be implemented to build  instantiable subclasses. RVL::FunctionalEvaluation and RVL::OperatorEvaluation are fully implemented. Their sanity-check input and output objects, and delegate the actual evaluation to the protected methods defined in RVL::Functional and RVL::Operator subclasses, after the pattern of the RVL::LinearOp apply... methods. Thus these user-defined protected methods can be written with assurance that all domain and range requirements are satisfied. 
<p>
The Evaluation class concept was introduced in the Hilbert Class Library and further developed in RVL. Besides automated sanity checks, the Evaluation device ensures that the various objects attached to a function at a point of its domain (value, derivative,...) remain coherent, and avoids a great deal of redundant effort otherwise necessary for correct programming. The evaluation technique has various consequences for the construction of functions, described in the  <a href="http://portal.acm.org/citation.cfm?doid=1499096.1499097">TOMS paper</a>.
  </li>
<p>
  <li><b>Data structures and low-level functions:</b> RVL::DataContainers typically do not expose their data - otherwise, out-of-core data, or data distributed over the net, would not be representable. Instead, interaction with data occurs through evaluation of <i>function objects</i> (functions with persistent state). RVL supplies two hierarchies of function objects, based at RVL::FunctionObject and RVL::FunctionObjectConstEval (according to whether evaluation may alter the RVL::DataContainer doing the evaluating, or not). RVL::DataContainer is also an abstract base, for which evaluation of the two types of function object must be defined in subclasses. These three hierarchies form an instance of the acyclic Visitor design pattern, with RVL::DataContainer as the Element type and the two function object classes as the Visitor types. Much of the programming effort in building an RVL application goes into designing and implementing RVL::DataContainer subclasses encapsulating various concrete data structures, and RVL::FunctionObject and RVL::FunctionObjectConstEval types to manipulate these data structures.
  </li>
<p>
<li>
<b>Auxiliary Constructs:</b>
<ul>
<li><b>Products:</b> Most scientific data of any complexity is <i>compound</i>, that is, consists of a number of component data structures. Linear algebraically, such data define points in Cartesian product vector spaces. RVL supplies an abstract notion of Cartesian product (RVL::Product, RVL::ROPRoduct), and partly (RVL::ProductSpace) and fully (RVL::StdProductSpace) implemented realizations of Cartesian product vector space using the product interface. Based on these, various specializations of the function classes (RVL::BlockOperator, RVL::FunctionalProductDomain) are also supplied. 
<p>
<li><b>Tests:</b> A great deal of application development time can be saved by elemental component tests. For computational realizations of linear and nonlinear functions, two of the most important are (1) adjoint test - check that implemented adjoint pairs of linear operators (RVL::LinearOp) are really adjoint to each other, at least to a modest multiple of machine precision, and (2) derivative test - check that computed derivative are consistent with divided differences, over some range of steps. RVL supplies implemented standalone functions (RVL::AdjointTest, RVL::DerivTest, RVL::GradientTest) for both purposes, which can be inserted into drivers with one or two lines of code, and produce formatted diagnostic output.
</li>
<p>
<li><b>Traits:</b> class template RVL::ScalarFieldTraits that abstracts the standard properties of scalar fields; use enables more portable code. For example, the float version of 1.0 is RVL::ScalarFieldTraits < float >::One(). The absolute value of a Complex < double > is a RVL::ScalarFieldTraits < Complex < double > > ::AbsType = double. 
</li>
<p>
<li><b>ProtectedDivision</b>: safeguarded division function template (RVL::ProtectedDivision), returns nonzero (true) if zerodivide occurs to floating precision.
</li>
</ul>
<p>
<li><b>Operator new:</b> For reasons explained in the design paper, generally no need arises to dynamcially allocate instances of the critical calculus types - RVL::Space, RVL::Vector, RVL::LinearOp, RVL::Functional, RVL::FunctionalEvaluation, RVL::Operator, and RVL::OperatorEvaluation. Therefore, operator new is by default protected (or, in one case, private) in these classes, and cannot be used except internally to the class and its close relatives (and in particular not in user code). However, it is conceivable that a case could arise in which dynamic allocation for one of these types is absolutely essential to accomplish some end, which simply cannot be attained using allocation on the stack. In view of this (unsubstantiated, as far as I know) possibility, RVL 1.0 permits the user to enable the standard compiler-defined operator new. Simply uncomment the definition of RVL_OPERATOR_NEW_ENABLED in rvl/rvl/include/utility.hh, and install. 
</ul>
<hr>
<a href="../../../doc/html/index.html">RVL Home Page</a>


  */