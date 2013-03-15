/*************************************************************************
Copyright (c) 2012 John McCaffery 

This file is part of NuiLib.

NuiLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NuiLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with NuiLib.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/
#include <NuiLib-API.h>

#ifndef EXTENDABLE_H
#define EXTENDABLE_H

using namespace NuiLib;

namespace NuiLib {

	///
	/// Root class for all components.
	/// Stores basic metadata about the instance. What type it is and what name it is.
	/// Components are the core building blocks of the library. They are designed to chain together. 
	/// A component is generally a wrapper around a simple type. The added behaviour is that, when the value is changed the component will trigger an event. 
	/// Any other components listening for these events will then know the value has been updated and update themselves according. Thus events can chain down through the system.
	/// Any new wrapper types should inherit from IComponent.
	///
	class DLL IComponent : public IObservable {
		private:
			string _type;
			string _name;

		protected:
			IComponent(string, string);

		public: 
			~IComponent();

			///
			/// Get the type of the component.
			/// Used for exporting from and importing to XML.
			///
			string GetType();
			///
			/// Get the name that identifies the component.
			/// Used for exporting from and importing to XML.
			///
			string GetName();
			///
			/// Get the name of the component as a C style string.
			/// Used for debugging.
			///
			const char *GetCName();
			///
			/// Set the name assigned to this object.
			/// @param name The name to assign to this component.
			///
			void SetName(string name);

			///
			/// Get a string that can be printed for information about this component.
			///
			virtual string ToString() = 0;
			///
			/// Get a string with a graphical representation of the component.
			/// The graphical representation is scaled to the specified length. 
			/// @param width The width to scale the graphical representation to. The string may be considerably longer than this value.
			/// 
			virtual string ToString(int width) = 0;
			///
			/// Write the component to a string of XML.
			///
			virtual string ToXML() =0;
			///
			/// Initialise the component based on an XML string.
			/// @param xml The XML string to load.
			///
			virtual void LoadXML(string xml) =0;

			///
			/// Whether this component only listens to triggers from the factory.
			/// Intended to be overrided by child classes.
			/// @return True means that if the component is a listener it will only ever have it's changed method called when the factory triggers. If false the component can listen to many sources for triggers.
			///
			inline static bool FactoryUpdated () { return false; }
			///
			/// Get the name of the type of the component.
			/// Intended to be overriden. Used for saving to and loading from XML.
			/// @return The name of the component.
			///
			inline static string GetTypeName() { return "IComponent"; };
	};

	///
	/// Wrapper around a scalar value.
	/// The value is stored as a float.
	/// Easiest to use with the dereference operator.
	/// IScalar x(5.f);
	/// float value = *x;
	/// X will equal 5.
	///
	/// IScalars are intended to be extended. They form the root of an inheritance heirarchy.
	/// New behaviour is created by extending them and implementing the Changed method.
	/// IScalars are dynamically allocated by an INuiFactory. They cannot be created any other way.
	/// They will be de-allocated by the factory.
	/// 
	class DLL IScalar : public IComponent, public IObserver {
		private:
			float _printMax;

		protected:
			///
			/// Initialise the scalar with type, name and values.
			/// Name will be "AnonymousScalar".
			/// @param type The type of the scalar.
			///
			IScalar(string type);

			///
			/// The value of this scalar.
			///
			float _value;

			///
			/// Calculate the value of the Scalar.
			/// @return The value for the scalar.
			///
			virtual float CalculateValue() { return _value; };

		public: 
			///
			/// Value = 0
			///
			IScalar();

			///
			/// Value = value
			///
			IScalar(string name, float value);

			///
			/// Dereference this Scalar to a float.
			/// @return Value.
			///
			float operator*();

			///
			/// Get the value of the scalar. 
			/// The * operator calls this method.
			/// This method can be overrided if necessary to provide extra behaviour.
			///
			virtual float Get();
			/// Set the value of this IScalar.
			///
			void Set(float);

			virtual string ToString();
			virtual string ToString(string);
			virtual string ToString(int);

			virtual string ToXML();
			virtual void LoadXML(string);

			void Changed(IObservable *source);

			inline static string GetTypeName() { return "IScalar"; };

			friend class INuiLib;
	};	
	
	const int X = 0;
	const int Y = 1;
	const int Z = 2;

	///
	/// 3D mathematical Vector class. 
	/// Wrapper around a 3D cartesian coordinate.
	/// Values are stored as floats.
	/// Values can be accessed using the [] operator.
	/// IVector vector(1.f, 2.f, 3.f);
	/// float x = vector[X_AXIS];
	/// float y = vector[Y_AXIS];
	/// float z = vector[Z_AXIS];
	///
	/// IVectors are intended to be extended. They form the root of an inheritance heirarchy.
	/// New behaviour is created by extending them and implementing the Changed method.
	/// IVectors are dynamically allocated by an INuiFactory. They cannot be created any other way.
	/// They will be de-allocated by the factory.
	/// 
	class DLL IVector : public IComponent, public IObserver {
		private:
			void Init(float, float, float);
			

		protected:
			///
			/// Initialise the vector with just its type.
			/// Name will be "AnonymousVector".
			/// @param type The type of the vector.
			///
			IVector(string type);			
			
			//float _values[3];
			///
			/// The value of this IVector.
			///
			cv::Point3f _value;
			///
			/// The maximum values of each axis and the magnitude. Used for output.
			float _maxs[4];

			///
			/// Make a string with a visual representation of one of the axes.
			/// @param width The width of the visual representation. The string may be considerably longer than this value.
			/// @param axis Which axis to display. x, y, z produce axes.
			/// @return A string with a visual representation of the axis value.
			///
			string MakeAxis(int width, char axis);
			///
			/// Make a string with a visual representation of the magnitude of the vector.
			/// @param width The width of the visual representation. The string may be considerably longer than this value.
			/// @return A string with a visual representation of the magnitude of this vector.
			///
			string MakeMagnitude(int width);

			///
			/// Calculate the value of the Scalar.
			/// @return The value for the scalar.
			///
			virtual cv::Point3f CalculateValue() { return _value; };

		public:
			///
			/// X = 0
			/// Y = 0
			/// Z = 0
			///
			IVector();

			///
			/// Dereference this vector to an open cv Point3f.
			///
			cv::Point3f operator*();
			///
			/// Index into the x, y or z value of this vector.
			/// Uses the X, Y, Z methods.
			///
			const float operator[](AXIS);

			///
			/// The x value of the vector.
			/// [] calls this. Can be overriden.
			///
			virtual float X();
			///
			/// The y value of the vector.
			/// [] calls this. Can be overriden.
			///
			virtual float Y();
			///
			/// The z value of the vector.
			/// [] calls this. Can be overriden.
			///
			virtual float Z();
			///
			/// The magnitude of this vector.
			///
			virtual float Magnitude();

			///
			/// Calculate the cross product of this vector and another vector.
			/// @param vector The other vector to calculate the cross product against.
			///
			virtual cv::Point3f Cross (IVector *vector);

			///
			/// Normalize this vector so that it has a magnitude of 1.
			///
			virtual cv::Point3f Normalize();

			///
			/// Set the x value of the vector.
			///
			virtual void X(float value);
			///
			/// Set the x value of the vector.
			///
			virtual void Y(float value);
			///
			/// Set the x value of the vector.
			///
			virtual void Z(float value);
			///
			/// Set all values of the vector.
			///
			virtual void Set(float x, float y, float z);

			string ToString();
			string ToString(int width);
			string ToString(int width, char);
			string ToXML();
			void LoadXML(string);

			void Changed(IObservable *source);

			inline static string GetTypeName() { return "IVector"; }

			friend class INuiLib;
	};

	///
	/// Wrapper around a boolean value.
	/// The value is stored as a bool.
	/// Easiest to use with the dereference operator.
	/// ICondition x(true);
	/// bool value = *x;
	/// X will be true.
	///
	/// IConditions are intended to be extended. They form the root of an inheritance heirarchy.
	/// New behaviour is created by extending them and implementing the Changed method.
	/// IConditions are dynamically allocated by an INuiFactory. They cannot be created any other way.
	/// They will be de-allocated by the factory.
	/// 
	class DLL ICondition : public IComponent, public IObserver {
	private:
		list<function<void (IObservable*)>> _trueListeners;
		list<function<void (IObservable*)>> _falseListeners;

	protected:
		bool _value;
		///
		/// Initialise the condition with type, name and value.
		/// Can only be accessed by an INuiFactory.
		/// Name will be "AnonymousCondition".
		/// @param type The type of the condition.
		///
		ICondition(string type);

		///
		/// Calculate the value of the Scalar.
		/// @return The value for the scalar.
		///
		virtual bool CalculateValue() { return _value; };

	public: 
		///
		/// Value = false
		///
		ICondition();
		///
		/// Value = value
		///
		ICondition(bool value);

		///
		/// Dereference this condition to a bool.
		///
		bool operator*();

		///
		/// The value of the condition.
		/// * calls this. Can be overriden.
		///
		virtual bool Get();
		///
		/// Set the value of the condition.
		///
		void Set(bool value);

		virtual string ToString();
		virtual string ToString(string);
		virtual string ToString(int);

		virtual string ToXML();
		virtual void LoadXML(string);

		void Changed(IObservable *source);
		///
		/// Add a listener that will called whenever this condition becomes true.
		///
		void OnTrue(function<void(IObservable*)> listener);
		///
		/// Add a listener that will called whenever this condition becomes false.
		///
		void OnFalse(function<void(IObservable*)> listener);

		inline static string GetTypeName() { return "ICondition"; };

		friend class INuiLib;
	};

	/// 
	/// The interface for factory classes which instantiate NuiLib objects.
	/// Contains generic template code which can instantiate any classes with the correct format.
	/// Deals with Vectors, Scalars and Conditions.
	/// Also provides some extra control over how polling is done.
	///
	class DLL INuiFactoryExtension : public INuiFactory {
	private: 
		map<string, function<IComponent *()>> _componentCreators;
		list<IComponent*> _components;
		set<IComponent*> _created;
		bool _destroying;

		template<typename T> T *MakeFromXML(string xml) {
			string name;
			string type;
			//TODO - extract name + type from XML
			T *item = Make<T>(name);
			item->LoadXML(this, xml);
			return item;
		}

		void UnregisterCreated(IComponent *);
		friend class IComponent;

	protected:
		///
		/// Initialise the factory.
		/// Specifies the type of the factory and its name.
		/// @param type The type of factory this is.
		/// @param name The name of this factory.
		///
		INuiFactoryExtension(string type, string name) : INuiFactory(type, name), _destroying(false) { }

		/// 
		/// Whether a factory triggors events on updates from a NuiDevice.
		/// Overloaded to determine whether the factory class which inherits from this interface will automatically trigger new events when data is received from the device.
		/// @return True if the factory will trigger on new input frames.
		///
		virtual bool Triggers() = 0;

		///
		/// Register a creator lambda.
		/// The creator lambda will be used to create new instances of the component of type T if one is requested whose type name matches 'type'.
		/// @param type The name of the type the lambda creates instances of. This will be found using T::GetName() when MakeVector is called.
		/// @param creator The lambda function which will create a new instance of Vectors looked up for name 'type'.
		///
		template<typename T> void RegisterCreator(string type, function<T*()> creator) {
			_componentCreators[type] = creator;
		}
		///
		/// Delete all components this factory had created.
		///
		void Clear();

	public:
		///
		/// Delete all components this factory created.
		/// Only components created using a Make method will be deleted. After this destructors has been called trying to use any components created by the factory will result in undefined behaviour.
		///
		virtual ~INuiFactoryExtension();

#ifdef VISUAL
		///
		/// Get the mat that will be displayed after each poll.
		///
		virtual cv::Mat &GetDebugFrame() = 0;
#endif		
		///
		/// True if there is a new colour frame.
		///
		virtual bool HasDepth() = 0;
		///
		/// True if there is a new colour frame.
		///
		virtual bool HasColour() = 0;
		///
		/// True if there is a new skelton frame.
		///
		virtual bool HasSkeleton() = 0;
		///
		/// Clear all resources associated with this factory.
		///
		virtual void Dispose() = 0;

		///
		/// Add a listener that will be notified of NUI events (Tick, SkeletonFound, SkeletonLost, SkeletonSwitched).
		///
		virtual void AddNuiListener(INuiListener *listener) = 0;		///
		/// Remove a listener that was being notified of NUI events.
		///
		virtual void RemoveNuiListener(INuiListener *listener) = 0;

		///
		/// Get a component with a given name.
		/// If there are more than one components which fit the criteria the one that was added first will be returned. If there are no components with the given name NULL is returned.
		/// @param The name to search for.
		/// @return The first component whose name matches name. NULL if none is found.
		///		
		template<typename T> T *Get(string name) {
			for (auto t = _components.begin(); t != _components.end(); t++) {
				if (name.compare((*t)->GetName()) == 0)
					return (T *)(*t);
			}
			return NULL;
		}
		///
		/// Make a component of type T with the given name.
		/// The component will be constructed with the default constructor (no arguments). If there is no 0 argument constructor for type T a compilation error will occur. Type name will be taken from the static method T::GetName(). If this method is not present the type will be named 'IVector'.
		/// @param name The name of the component to create.
		/// @return The new component of type T.
		///
		template<typename T> T *Make(string name) {
			T *item = Get<T>(name);
			if (item)
				return item;
			string type = T::GetTypeName();
			if (_componentCreators.count(type) > 0)
				item = (T*)_componentCreators[type]();
			else
				item = new T();
			item->SetName(name);
			Add(item);
			_created.insert(item);
			return item;
		}
		///
		/// Add a component to the factory.
		/// If the component overrides the static method FactoryUpdated() to return true the component will be set to only updated when the factory triggors and update.
		/// @param item The component to add.
		///		
		template<typename T> void Add(T *item) {
			_components.push_back(item);
			if (Triggers() && T::FactoryUpdated()) 
				((IObserver *)item)->SetFactoryObserver(true);
		}

		Scalar GetScalar(string name) {
			return Scalar(Get<IScalar>(name));
		}
		Condition GetCondition(string name){
			return Condition(Get<ICondition>(name));
		}
		Vector GetVector(string name){
			return Vector(Get<IVector>(name));
		}

		string ToXML() { return ""; }
		void LoadXML(string xml) { }
	};

	///
	/// Get the factory which can be used to interact with the NUI device.
	///
	DLL INuiFactoryExtension *ExtensionFactory();
}

#endif
