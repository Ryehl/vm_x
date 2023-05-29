#pragma once

namespace android {
    namespace featurecontrol {
        enum Feature {
            GLPipeChecksum,
            ForceANGLE,
            ForceSwiftshader,
            HYPERV,
            HVF,
            KVM,
            HAXM,
            FastSnapshotV1,
            ScreenRecording,
            VirtualScene,
            IgnoreHostOpenGLErrors,
            GenericSnapshotsUI,
            AllowSnapshotMigration,
            WindowsOnDemandSnapshotLoad,
            WindowsHypervisorPlatform,
            OnDemandSnapshotLoad,
            GrallocSync,
            EncryptUserData,
            IntelPerformanceMonitoringUnit,
            GLAsyncSwap,
            GLDMA,
            GLESDynamicVersion,
            Wifi,
            PlayStoreImage,
            LogcatPipe,
            SystemAsRoot,
            Feature_n_items
        };
    }
}
