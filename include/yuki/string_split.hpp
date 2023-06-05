#pragma once
#include<string>
#include<stdexcept>
#include<yuki/Pair.hpp>

namespace yuki{
template<typename C,typename Tr,typename A=std::allocator<C>>
yuki::Pair<std::basic_string<C,Tr,A>,std::basic_string<C,Tr,A>> split_first(const std::basic_string_view<C,Tr> source,const std::type_identity_t<C> character) noexcept(false) {
    const typename std::basic_string_view<C,Tr>::size_type pos=source.find(character);
    if(pos==std::basic_string_view<C,Tr>::npos)
        throw std::runtime_error("yuki::split_first: Split-character not found!");
    return {source.substr(0,pos),source.substr(pos+1,source.size())};
}
template<typename C,typename Tr=std::char_traits<C>,typename A=std::allocator<C>,typename View,typename=std::enable_if_t<std::is_convertible_v<const View&,std::basic_string_view<C,Tr>>>>
yuki::Pair<std::basic_string<C,Tr,A>,std::basic_string<C,Tr,A>> split_first(const View& source,const C character) noexcept(false) {
    const std::basic_string_view<C,Tr> sv=source;
    return split_first<C,Tr,A>(sv,character);
}

template<typename C,typename Tr,typename A=std::allocator<C>>
yuki::Pair<std::basic_string<C,Tr,A>,std::basic_string<C,Tr,A>> split_last(const std::basic_string_view<C,Tr> source,const std::type_identity_t<C> character) noexcept(false) {
    const typename std::basic_string_view<C,Tr>::size_type pos=source.rfind(character);
    if(pos==std::basic_string_view<C,Tr>::npos)
        throw std::runtime_error("yuki::split_last: Split-character not found!");
    return {source.substr(0,pos),source.substr(pos+1,source.size())};
}
template<typename C,typename Tr=std::char_traits<C>,typename A=std::allocator<C>,typename View,typename=std::enable_if_t<std::is_convertible_v<const View&,std::basic_string_view<C,Tr>>>>
yuki::Pair<std::basic_string<C,Tr,A>,std::basic_string<C,Tr,A>> split_last(const View& source,const C character) noexcept(false) {
    const std::basic_string_view<C,Tr> sv=source;
    return split_last<C,Tr,A>(sv,character);
}

template<typename C,typename Tr>
yuki::Pair<std::basic_string_view<C,Tr>,std::basic_string_view<C,Tr>> vsplit_first(const std::basic_string_view<C,Tr> source,const std::type_identity_t<C> character) noexcept(false) {
    const typename std::basic_string_view<C,Tr>::size_type pos=source.find(character);
    if(pos==std::basic_string_view<C,Tr>::npos)
        throw std::runtime_error("yuki::vsplit_first: Split-character not found!");
    return {source.substr(0,pos),source.substr(pos+1,source.size())};
}
template<typename C,typename Tr=std::char_traits<C>,typename View,typename=std::enable_if_t<std::is_convertible_v<const View&,std::basic_string_view<C,Tr>>>>
yuki::Pair<std::basic_string_view<C,Tr>,std::basic_string_view<C,Tr>> vsplit_first(const View& source,const C character) noexcept(false) {
    const std::basic_string_view<C,Tr> sv=source;
    return vsplit_first<C,Tr>(sv,character);
}

template<typename C,typename Tr>
yuki::Pair<std::basic_string_view<C,Tr>,std::basic_string_view<C,Tr>> vsplit_last(const std::basic_string_view<C,Tr> source,const std::type_identity_t<C> character) noexcept(false) {
    const typename std::basic_string_view<C,Tr>::size_type pos=source.rfind(character);
    if(pos==std::basic_string_view<C,Tr>::npos)
        throw std::runtime_error("yuki::vsplit_last: Split-character not found!");
    return {source.substr(0,pos),source.substr(pos+1,source.size())};
}
template<typename C,typename Tr=std::char_traits<C>,typename View,typename=std::enable_if_t<std::is_convertible_v<const View&,std::basic_string_view<C,Tr>>>>
yuki::Pair<std::basic_string_view<C,Tr>,std::basic_string_view<C,Tr>> vsplit_last(const View& source,const C character) noexcept(false) {
    const std::basic_string_view<C,Tr> sv=source;
    return vsplit_last<C,Tr>(sv,character);
}

template<typename C,typename Tr,typename A=std::allocator<C>>
yuki::Pair<std::basic_string<C,Tr,A>,std::basic_string<C,Tr,A>> split_filename(const std::basic_string_view<C,Tr> filename) noexcept {
    const typename std::basic_string_view<C,Tr>::size_type pos=filename.rfind('.');
    if(pos==std::basic_string_view<C,Tr>::npos)
        return {{filename.data(),filename.size()},{}};
    return {filename.substr(0,pos),filename.substr(pos+1,filename.size())};
}
template<typename C,typename Tr=std::char_traits<C>,typename A=std::allocator<C>,typename View,typename=std::enable_if_t<std::is_convertible_v<const View&,std::basic_string_view<C,Tr>>>>
yuki::Pair<std::basic_string<C,Tr,A>,std::basic_string<C,Tr,A>> split_filename(const View& source) noexcept {
    const std::basic_string_view<C,Tr> sv=source;
    return split_filename<C,Tr,A>(sv);
}

template<typename C,typename Tr>
yuki::Pair<std::basic_string_view<C,Tr>,std::basic_string_view<C,Tr>> vsplit_filename(const std::basic_string_view<C,Tr> filename) noexcept {
    const typename std::basic_string_view<C,Tr>::size_type pos=filename.rfind('.');
    if(pos==std::basic_string_view<C,Tr>::npos)
        return {filename,{}};
    return {filename.substr(0,pos),filename.substr(pos+1,filename.size())};
}
template<typename C,typename Tr=std::char_traits<C>,typename View,typename=std::enable_if_t<std::is_convertible_v<const View&,std::basic_string_view<C,Tr>>>>
yuki::Pair<std::basic_string_view<C,Tr>,std::basic_string_view<C,Tr>> vsplit_filename(const View& source) noexcept {
    const std::basic_string_view<C,Tr> sv=source;
    return vsplit_filename<C,Tr>(sv);
}
} // namespace yuki
