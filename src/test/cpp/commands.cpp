#include "CppUnitTest.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/commands.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	class BluntFeedItem : public IFeedItem {
	public:
		BluntFeedItem(wstring title) : title(title) {}

		virtual HRESULT STDMETHODCALLTYPE Xml(FEEDS_XML_INCLUDE_FLAGS includeFlags, __RPC__deref_out_opt BSTR *xml) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Title(__RPC__deref_out_opt BSTR *title) {
			*title = SysAllocString(this->title.c_str()); return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE get_Link(__RPC__deref_out_opt BSTR *linkUrl) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Guid(__RPC__deref_out_opt BSTR *itemGuid) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Description(__RPC__deref_out_opt BSTR *description) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_PubDate(__RPC__out DATE *pubDate) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Comments(__RPC__deref_out_opt BSTR *comments) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Author(__RPC__deref_out_opt BSTR *author) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Enclosure(__RPC__deref_out_opt IDispatch **disp) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_IsRead(__RPC__out VARIANT_BOOL *isRead) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE put_IsRead(VARIANT_BOOL isRead) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_LocalId(__RPC__out LONG *itemId) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Parent(__RPC__deref_out_opt IDispatch **disp) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE Delete(void) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_DownloadUrl(__RPC__deref_out_opt BSTR *itemUrl) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_LastDownloadTime(__RPC__out DATE *lastDownload) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Modified(__RPC__out DATE *modified) { return E_NOTIMPL; }

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &, void **) { return E_NOTIMPL; }
		virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }
		virtual ULONG STDMETHODCALLTYPE Release(void) { return 0; }
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo **) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) { return E_NOTIMPL; }
	private:
		wstring title;
	};

	TEST_CLASS(Commands)
	{
	public:

		TEST_METHOD(FeedItem_DetailsConatainsTitle) {
			::FeedItem feedItem(new BluntFeedItem(L"The Title"));
			Assert::AreNotEqual(wstring::npos, feedItem.getDetailsString().find(L"The Title"));
		}
	};
}
