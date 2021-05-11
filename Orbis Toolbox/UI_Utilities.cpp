#include "Common.h"
#include "UI.h"

void UI::Utilities::SetVersionString(const char* str)
{
	MonoClass* SystemSoftwareVersionInfo = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "SystemSoftwareVersionInfo");
	Mono::Set_Property(SystemSoftwareVersionInfo, Mono::Get_Instance(SystemSoftwareVersionInfo, "Instance"), "DisplayVersion", Mono::New_String(str));
}

void UI::Utilities::ReloadItemList()
{
	MonoClass* ContentsAreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "ContentsAreaManager");
	MonoClass* ContentsList = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "ContentsList");

	MonoObject* m_scene = Mono::Get_Field<MonoObject*>(ContentsAreaManager, Mono::Get_Instance(ContentsAreaManager, "Instance"), "m_scene");
	MonoArray* m_contentsList = Mono::Get_Field<MonoArray*>(Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "ContentAreaScene"), m_scene, "m_contentsList");
	MonoObject* m_contentsList_0 = mono_array_get(m_contentsList, MonoObject*, 0);

	if (m_contentsList_0)
		Mono::Invoke<void>(Mono::App_exe, ContentsList, m_contentsList_0, "ReloadItemSource");
}

MonoObject* UI::Utilities::NewAppBrowseItem(const char* TitleId, const char* TitleName)
{
	MonoObject* Instance = Mono::New_Object(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItem");
	mono_runtime_object_init(Instance);

	Mono::Set_Property(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemLite", Instance, "TitleId", Mono::New_String(TitleId));
	Mono::Set_Property(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemBase", Instance, "TitleName", Mono::New_String(TitleName));
	Mono::Set_Property(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemBase", Instance, "MetaDataPath", 0);

	return Instance;
}

MonoObject* UI::Utilities::Get_Top_Scene()
{
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	return Mono::Invoke<MonoObject*>(Mono::App_exe, AreaManager, Mono::Get_Instance(AreaManager, "Instance"), "GetTopScene");
}

MonoObject* UI::Utilities::Get_root_Widget()
{
	return Mono::Get_Property<MonoObject*>(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "Scene", Get_Top_Scene(), "RootWidget");
}

MonoObject* UI::Utilities::Adjust_Content(int AlignOrientation, float PaddingLeft, float PaddingRight, float PaddingTop, float PaddingBottom)
{
	MonoClass* AdjustContent = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "AdjustContent");

	//Allocates memory for our new instance of a class.
	MonoObject* AdjustContent_Instance = Mono::New_Object(AdjustContent);

	//Call the default no param constructor.
	mono_runtime_object_init(AdjustContent_Instance);

	//Add Properties.
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "AlignOrientation", AlignOrientation);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingLeft", PaddingLeft);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingRight", PaddingRight);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingTop", PaddingTop);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingBottom", PaddingBottom);

	return AdjustContent_Instance;
}

MonoObject* UI::Utilities::Fit_To_Children()
{
	MonoClass* FitToChildren = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "FitToChildren");

	MonoObject* FitToChildren_Instance = Mono::New_Object(FitToChildren);
	mono_runtime_object_init(FitToChildren_Instance);

	return FitToChildren_Instance;
}

MonoObject* UI::Utilities::IUFont(int size, int style, int weight)
{
	MonoClass* UIFont = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "UIFont");

	//Allocates memory for our new instance of a class.
	MonoObject* UIFont_Instance = Mono::New_Object(UIFont);
	Mono::Invoke<void>(Mono::App_exe, UIFont, (MonoObject*)mono_object_unbox(UIFont_Instance), ".ctor", size, style, weight);

	return (MonoObject*)mono_object_unbox(UIFont_Instance);
}