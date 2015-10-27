Overview
--------
Prerequisites
-------------
To be able to work with AWind library the basic understanding of object oriented programming (OOP) is very helpful. You need to know what class means, you should understand class inheritance concept, member function, member variables. Nevertheless, deep understanding of OOP for the working with AWind library isn't necessary. AWind library uses also templates but very limited. So if you don't understand templates consider, them as wonder :-) 

Concepts
-------- 
The AWind library is designed in similar way like other window libraries on 'big' systems (e.g. Windows). Many efforts are put on optimisation of SRAM-memory consumption. 
SRAM-memory is on-board memory where runtime variables are stored. This memory in comparison to 'big' systems is very limited resource.
Other direction of optimization is reduction of display updates. Arduino TFT displays need time to redraw. To reduce flickering only corresponding parts of display are updated - like text fields or buttons and etc.
* Window: It can take full screen or its part, it can be a child of other window. Coordinates of child window are always relative to the parent window.
  Window can be visible or hidden. Appearance of windows can be implemented in two ways: by implementing of OnDraw function (see ChartWindow class) or what is recommended if it applicable via decorators (about decorators see below) Everything is more or less the same like in the 'big' systems. 
* Dialog: special type of window where input of information from user is expected. If dialog window is active there is no way to interact with other windows
* Controls: Button, text box, chart, gauges gauges. 
* Decorator: Decorators defines window appearance. Decorator can be shared between windows of the same type like buttons.
  Decorators play important role in the saving of SRAM memory, that's way they should be used instead of implementing of OnDraw member function. 
* DC: Device context - abstraction layer between device drawing code and AWind library.
  Currently such layer is implemented only for UTFT library, but in general by implemented of specific DC class, AWind library can be ported/adjusted to other TFT libraries of even to the other than Arduino platforms.
* Events: Communication between different windows especially between controls and parent window or between sensor and window runs over functions that are call in target window if something with control happens:
  button is pressed, text is changed and etc.. To connect event generator and event receiver the class of window receiver has to be derived  from corresponding interface class (IContentChangedEventReceiver, IDialogClosedEventReceiver and etc. name of such classes begins with 'I') and then has to be registered  window in the control. 
* MainWindow. Application window. The lowest window in the application. All other windows are children or children of children or ... . Usually AWind library user derives from MainWindow his own MainWindow class.

Application
--------
To create your own application based on AWind library following steps need to be done
1. Define Main window. You can use already existent MainWindow or derive your own class from MainWindow
2. Create child windows. Combining different controls you can generate your own application look. Usually It is done in the constructor of parent window
3. Additionally derive you window from corresponding interface classes (see Events) if you need to process events from controls or sensors  

In general to start you should select example that is close to your needs and extend it with your functionality.

That's all you need to know about AWind internals to use AWind library efficiently. If you have still difficulties just ask.
