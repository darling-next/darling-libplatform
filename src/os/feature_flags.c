// created for Darling
//
// _os_feature_enabled_impl -- the function behind os_feature_enabled()/os_feature_enabled_simple().
// The Xcode 26.3 guest ladder stopped here (dar- wave): XCTAutomationSupport.framework imports the
// name from /usr/lib/libSystem.B.dylib, whose deployed re-export closure carried no provider, so the
// loader could not bind it.  (Apple's own macOS 26 and iOS 26 SDK tbd files name
// /usr/lib/system/libsystem_featureflags.dylib as the owner; this tree has no such library, and the
// name is reachable through libSystem.B either way, so it lives here with the rest of the os_
// primitives -- the family's own home before Apple split it out, next to the os_* headers this
// library already installs.)
//
// WHY "DISABLED" IS THE HONEST ANSWER FOR EVERY DOMAIN AND FEATURE
//   Darling implements no feature-flag machinery at all: there is no flag registration, no
//   FeatureFlags/GlobalDisableFeatures plist ingestion, and no per-process feature override in this
//   tree.  The tree already says the same thing in the form a Darling-compiled client sees --
//
//     Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/os/feature_private.h
//         #define os_feature_enabled(framework, feature) (0)
//         #define os_feature_enabled_simple(framework, feature, boolean) (0)
//
//   -- i.e. "disabled" is the platform's existing, declared answer for every feature any guest asks
//   about, and it is truthful: this tree genuinely does not implement the feature, so a client that
//   asks about one gets the same answer a Darling-compiled client gets through the header.  Answering
//   "enabled" would invent a feature, which is the one thing this must not do.
//
// THE CLIENT, MEASURED (not assumed)
//   XCTAutomationSupport.framework/Versions/A/XCTAutomationSupport is the only image of the ladder's
//   254-image load closure that imports the name, and it calls it exactly once, at 0x16dc5:
//
//       leaq ..(%rip), %rdi   ## literal pool for: "SwiftUI"    (domain)
//       leaq ..(%rip), %rsi   ## literal pool for: "Solarium"   (feature)
//       callq 0x400e0 ## symbol stub for: __os_feature_enabled_impl
//       testb %al, %al
//       je    <the ordinary image-export path>
//
//   Two C strings, a _Bool result, read once and branched on: "disabled" takes the default path,
//   which is where Darling has always been, and nothing is written through a pointer.
//
//   _os_feature_enabled_simple_impl (the family's other member, per Apple's tbd) is NOT implemented
//   here: no image of the load closure imports it, so its existence cannot be measured from the
//   guest and neither can its call shape, and guessing an ABI from memory is worse than leaving a
//   name nothing asks for absent.

_Bool
_os_feature_enabled_impl(const char *domain, const char *feature)
{
	(void)domain;
	(void)feature;
	return 0;
}
